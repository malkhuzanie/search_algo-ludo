#ifndef UTILS_H
#define UTILS_H

#include <format>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#elif defined(__linux__)
#include <sys/ioctl.h>
#endif // Windows/Linux

inline void get_terminal_size(int &width, int &height) {
#if defined(_WIN32)
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  width = (int)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
  height = (int)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
#elif defined(__linux__)
  struct winsize w;
  ioctl(fileno(stdout), TIOCGWINSZ, &w);
  width = (int)(w.ws_col);
  height = (int)(w.ws_row);
#endif // Windows/Linux
}

inline void print_centered(const std::string &text) {
  int rows, cols;
  get_terminal_size(cols, rows);
  int x = (cols - text.length()) / 2; // Center horizontally
  std::cout << std::string(x, ' ') << text << std::endl;
}

inline std::string center(const std::string &s) {
  int rows, cols;
  get_terminal_size(cols, rows);
  return std::format("{:^{}}", s, cols);
}

inline std::string center(const std::string &s, size_t width) {
  return std::format("{:^{}}", s, width);
}

inline int terminal_width() {
  int rows, cols;
  get_terminal_size(cols, rows);
  return cols;
}

inline string dotted_line(const string &text) { return "⚫" + text; }

inline int get_padding(const std::string &s) {
  int rows, cols;
  get_terminal_size(cols, rows);
  return (cols - s.size()) >> 1;
}

inline string get_sep(const char &ch, const int &len) {
  return string(len, ch);
}

inline void print_sep(const char &ch, const int &len) {
  cout << string(len, ch) << '\n';
}

#endif
