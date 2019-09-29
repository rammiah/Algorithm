#include <iostream>
#include <atomic>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>


class shared_mutex {
private:
    // 内部互斥锁，用与互斥访问_state
    std::mutex _mut;
    // 读者写者都用
    std::condition_variable _gate1;
    // 唤醒读者
    std::condition_variable _gate2;
    // 记录当前状态，进入写状态的写者数目和读者数目
    // 4字节，4 * 8 - 1 = 31，1<<31就是只有最高位是1的数字
    static const unsigned _writer_entered = 1U << (sizeof(unsigned) * 8 - 1);
    const static unsigned _readers_cnt = ~_writer_entered;
    // 当前状态，可以使用上面的两个常量获取状态，是否安全我就不知道了
    unsigned _state;

    bool no_writer() {
        return (_state & _writer_entered) == 0;
    }

    bool no_readers() {
        return (_state & _readers_cnt) == 0;
    }

    bool no_reader_no_writer() {
        return _state == 0;
    }

    bool no_writer_no_max_reader() {
        return (_state & _writer_entered) == 0 && (_state & _readers_cnt) != _readers_cnt;
    }


public:
    shared_mutex() noexcept;

    ~shared_mutex();

    void lock();

    void unlock();

    bool try_lock();


    void lock_shared();

    void unlock_shared();

    bool try_lock_shared();

};


void shared_mutex::lock() {
    std::unique_lock<std::mutex> lk{_mut};
    // 等待写者退出或者说排队
    _gate1.wait(lk, std::bind(&shared_mutex::no_writer, this));
    _state |= _writer_entered;
    // 等待已进入的读者出来
    _gate2.wait(lk, std::bind(&shared_mutex::no_readers, this));
}

void shared_mutex::unlock() {
    // 解锁吧
    std::unique_lock<std::mutex> lk;
    // 此时没有读者，但不确定有没有读者等待写者进入
    _state = 0;
    // 提醒写者和读者，所以就是随机唤醒的。。。
    _gate1.notify_all();
}

bool shared_mutex::try_lock() {
    std::unique_lock<std::mutex> lk{_mut};
    if (no_reader_no_writer()) {
        _state |= _writer_entered;
        return true;
    }

    return false;
}

void shared_mutex::lock_shared() {
    // 写优先
    std::unique_lock<std::mutex> lk{_mut};
    // 首先排在写锁后面
    _gate1.wait(lk, std::bind(&shared_mutex::no_writer_no_max_reader, this));
    // 最高位为0，直接加1就行了
    ++_state;
}

void shared_mutex::unlock_shared() {
    std::unique_lock<std::mutex> lk{_mut};
    --_state;
    unsigned readers = _state & _readers_cnt;
    // 写者减1
    if (no_writer()) {
        // 队原来满了
        if (readers == _readers_cnt - 1) {
            _gate1.notify_one();
        }
    } else {
        // 没读者了，唤醒写者
        if (readers == 0) {
            _gate2.notify_one();
        }
    }
}

bool shared_mutex::try_lock_shared() {
    std::unique_lock<std::mutex> lk{_mut};
    if (no_writer_no_max_reader()) {
        ++_state;
        return true;
    }

    return false;
}

shared_mutex::shared_mutex() noexcept : _state(0) {

}

shared_mutex::~shared_mutex() {
    // 在这里等待所有写者和读者退出，并且禁止读者进入
    std::unique_lock<std::mutex> lk{_mut};
    _gate1.wait(lk, std::bind(&shared_mutex::no_writer, this));
    // 然后等待读者出去
    _state |= _writer_entered;
    _gate2.wait(lk, std::bind(&shared_mutex::no_readers, this));
    // 然后结束了
}

// 两个工具类
template<typename Mutex>
class lock_guard {
private:
    Mutex &m;
public:
    explicit lock_guard(Mutex &_m);

    ~lock_guard();
};

template<typename Mutex>
lock_guard<Mutex>::lock_guard(Mutex &_m) : m(_m) {
    m.lock();
}

template<typename Mutex>
lock_guard<Mutex>::~lock_guard() {
    m.unlock();
}

template<typename Mutex>
class shared_lock_guard {
private:
    Mutex &m;
public:
    explicit shared_lock_guard(Mutex &_m);

    ~shared_lock_guard();
};

template<typename Mutex>
shared_lock_guard<Mutex>::shared_lock_guard(Mutex &_m) : m(_m) {
    m.lock_shared();
}

template<typename Mutex>
shared_lock_guard<Mutex>::~shared_lock_guard() {
    m.unlock_shared();
}

volatile int val = 0;
shared_mutex mut;
std::atomic_bool end{false};

void read(int id) {
    while (!end) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        shared_lock_guard<shared_mutex> gd{mut};
        printf("%d val = %d\n", id, val);
    }
}

void write(int id) {
    while (!end) {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        lock_guard<shared_mutex> gd{mut};
        ++val;
        printf("%d val add 1\n", id);
    }
}

int main() {
    std::vector<std::thread> ts;
    // 线程集合
    for (int i = 0; i < 7; ++i) {
        ts.emplace_back(read, i);
    }
    for (int i = 0; i < 2; ++i) {
        ts.emplace_back(write, i * 10);
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));
    end = true;

    for (auto &t : ts) {
        t.join();
    }

    return 0;
}
