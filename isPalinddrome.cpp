#include <iostream>
#include <string>
using namespace std;

// 문자열이 회문인지 확인하는 함수
bool isPalindrome(const string& str, int start, int end) {
    // 기본 종료 조건: 문자열의 시작과 끝이 만나거나 교차하면 회문
    if (start >= end)
        return true;

    // 시작과 끝 문자가 다르면 회문이 아님
    if (str[start] != str[end])
        return false;

    // 재귀적으로 다음 문자 확인
    return isPalindrome(str, start + 1, end - 1);
}

// 재귀 함수로 문자열을 뒤집는 함수
string reverseString(const string& str, int index = 0) {
    static string reversed = ""; // 뒤집힌 문자열 저장
    if (index == str.size()) // 모든 문자를 처리했으면 결과 반환
        return reversed;

    reversed = str[index] + reversed; // 현재 문자를 앞에 추가
    return reverseString(str, index + 1); // 다음 문자 처리
}

int main() {
    string input;
    cout << "문자열을 입력하세요: ";
    cin >> input;

    // 회문인지 확인
    if (isPalindrome(input, 0, input.size() - 1)) {
        cout << "입력한 문자열은 회문입니다!" << endl;
    }
    else {
        cout << "입력한 문자열은 회문이 아닙니다." << endl;
    }

    // 문자열 뒤집기
    string reversed = reverseString(input);
    cout << "뒤집힌 문자열: " << reversed << endl;

    return 0;
}
