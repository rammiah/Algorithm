
use std::collections::HashMap;
use std::fs::File;
use std::io::{prelude::*, BufReader};

pub struct Trie {
    words: HashMap<char, Trie>,
    end: bool,
}

impl Trie {
    pub fn new() -> Trie {
        Trie {
            words: HashMap::new(),
            end: false,
        }
    }

    pub fn from_file(filename: &str) -> std::io::Result<Trie> {
        let mut t = Trie::new();
        // 打开文件
        let f = File::open(filename)?;
        let f = BufReader::new(f);

        for line in f.lines() {
            t.add(&line.unwrap());
        }

        Ok(t)
    }

    pub fn add(&mut self, word: &str) {
        if word.is_empty() {
            self.end = true;
            return;
        }
        match self.words.get_mut(&word.chars().next().unwrap()) {
            Some(t) => {
                // 此时是一个Trie
                t.add(&word[1..]);
            }
            None => {
                let mut t = Trie::new();
                // 需要先添加单词再插入map
                // 否则会报所有权已转移错误
                t.add(&word[1..]);
                self.words.insert(word.chars().next().unwrap(), t);
            }
        }
    }

    pub fn contains(&self, word: &str) -> bool {
        if word.is_empty() {
            return self.end;
        }
        match self.words.get(&word.chars().next().unwrap()) {
            Some(t) => t.contains(&word[1..]),
            None => false,
        }
    }

    fn starts_with_helper(&self, prefix: &str, word: &mut String, words: &mut Vec<String>) {
        if prefix.is_empty() {
            // 当前的判定
            if self.end {
                words.push(word.clone());
            }
            // 遍历后面的
            for (c, t) in self.words.iter() {
                word.push(*c);
                t.starts_with_helper(prefix, word, words);
                word.pop();
            }

            return;
        }
        let ch = prefix.chars().next().unwrap();
        match self.words.get(&ch) {
            Some(t) => {
                word.push(ch);
                t.starts_with_helper(&prefix[1..], word, words);
                word.pop();
            }
            None => return,
        }
    }

    pub fn starts_with(&self, word: &str) -> Vec<String> {
        // 需要写一个helper了
        let mut v = Vec::new();
        let mut s = String::new();
        self.starts_with_helper(word, &mut s, &mut v);
        v
    }
}

#[cfg(test)]
mod tests {

    use super::*;

    #[test]
    fn test_starts_with() {
        let mut t = Trie::new();
        t.add("hello");
        t.add("bad");
        t.add("bool");

        assert_eq!(t.starts_with("h"), vec!["hello"]);
        assert_eq!(t.starts_with("ba"), vec!["bad"]);
        // 需要进行排序操作
        let mut result = t.starts_with("b");
        result.sort();
        assert_eq!(result, vec!["bad", "bool"]);
    }

    #[test]
    fn test_add_contains() {
        let mut t = Trie::new();
        let words = vec!["hello", "world", "java", "not"];

        for word in words {
            t.add(&word);
        }

        assert_eq!(t.contains("hello"), true);
        assert_eq!(t.contains("world"), true);
        assert_eq!(t.contains("javascript"), false);
        assert_eq!(t.contains("not"), true);
    }
}
