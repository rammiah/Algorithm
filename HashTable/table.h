#pragma once
#include "iostream"    
using namespace std;

template<class DataType>
class HashTable{
public:
	HashTable(int size){
		maxSize = size;
		count = 0;
		element = new DataType[size];
		if (element == NULL) {
			exit(1);
		}
		else {
			for (int i = 0; i<size; i++){
				element[i] = NULL;
			}
		}
	}
	~HashTable(){
		delete[] element;
	}
	DataType getData(int i){
		if (i < 0 || i >= maxSize){
			exit(1);
		}
		else{
			return element[i];
		}

	}

	int getNum(){
		return count;
	}
	//哈希函数，返回ndata的(在数组中的)存储位置    
	int Hash(DataType nData);
	//插入操作,插入newData到哈希表中    
	int insertHash(DataType newData);
	//搜寻nData的当前存储位置    
	int getHash(DataType nData);
private:
	int maxSize;
	int count;
	DataType *element;
};


template<class DataType>
int HashTable<DataType>::Hash(DataType newData){
	return newData%maxSize;//留余数法    
}

//搜索nData的空白存储位置    
template<class DataType>
int HashTable<DataType>::insertHash(DataType newData){
	if (getNum() == maxSize){
		cerr << "哈希表已经满" << endl;
	}
	int pos = Hash(newData);
	if (element[pos] == NULL){
		element[pos] = newData;
		count++;
		return 1;
	}
	else{
		int rpos = (pos + 1) % maxSize;
		while (rpos != pos){
			if (element[rpos] == NULL)//寻找没有存储数据的位置    
			{
				element[rpos] = newData;
				count++;
				return 1;
			}
			rpos = (rpos + 1) % maxSize;
		}

		if (rpos == pos)//如果最终都没能找到空位，直接强行赋值    
		{
			element[rpos] = newData;
			count++;
			return 1;
		}
	}
}

//搜寻nData的当前存储位置    
template<class DataType>
int HashTable<DataType>::getHash(DataType newData){
	int pos = Hash(newData);
	if (element[pos] == newData){
		return pos;
	}
	else{
		int rpos = (pos + 1) % maxSize;
		while (rpos != pos){
			if (element[rpos] == newData){
				return rpos;
			}
			rpos = (rpos + 1) % maxSize;
		}
		if (rpos == pos){
			cerr << "无法查找指定元素的存储位置" << endl;
			return -1;
		}
		else{
			return rpos;
		}
	}
}
