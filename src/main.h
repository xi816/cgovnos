// Basic function declarations for the main file

void old_st(struct termios oldt) {
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

