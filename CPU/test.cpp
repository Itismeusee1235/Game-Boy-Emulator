#include "./cpu.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
  CPU cpu;
  int flag = 0;
  int value = 0;
  int ch = 0;
  while (true) {
    cout << ">";
    cin >> ch;
    switch (ch) {
    case 1: {
      cout << "Flag: ";
      cin >> flag;
      cout << "Value: ";
      cin >> value;
      cpu.set_flag(flag, value);
      break;
    }
    case 2: {
      cout << "Flag: " << endl;
      cin >> flag;
      cout << cpu.get_flag(flag);
      break;
    }
    case 3: {
      cout << "Flag: ";
      cin >> flag;
      cout << "Value: ";
      cin >> value;
      cpu.set_reg8(flag, static_cast<uint8_t>(value));
      break;
    }
    case 4: {
      cout << "Flag: ";
      cin >> flag;
      cout << "Value: ";
      cin >> value;
      cpu.set_reg16(flag, static_cast<uint16_t>(value));
      break;
    }
    case 5: {
      cout << "Flag: " << endl;
      cin >> flag;
      cout << static_cast<int>(cpu.get_reg8(flag));
      break;
    }
    case 6: {
      cout << "Flag: " << endl;
      cin >> flag;
      cout << static_cast<int>(cpu.get_reg16(flag));
      break;
    }
    case 7: {
      int src, des;
      cout << "Reg 1 index ";
      cin >> des;
      cout << "Reg 2 index ";
      cin >> src;
      cpu.load_reg_reg(des, src);
    }
    default: {
      cpu.print_reg();
    }
    }
  }
  return 0;
}
