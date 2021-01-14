#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#define MAX 1000
#define CONTACT_NAME "contact.txt"

using namespace std;

struct Person {
  string name;
  int age;
  int sex;
  string phone;
  string addr;
};

struct AddressBook {
  int size;
  struct Person persons[MAX];
};

void showMenu() {
  cout << "*************************" << endl;
  cout << "***** 1) 添加联系人 *****" << endl;
  cout << "***** 2) 显示联系人 *****" << endl;
  cout << "***** 3) 删除联系人 *****" << endl;
  cout << "***** 4) 查找联系人 *****" << endl;
  cout << "***** 5) 修改联系人 *****" << endl;
  cout << "***** 6) 清空联系人 *****" << endl;
  cout << "***** 0) 退出通讯录 *****" << endl;
  cout << "*************************" << endl;
}

/*
* 从文本加载联系人信息
*/
void loadContact(AddressBook *ab) {
  ifstream contact;

  contact.open((const char*)CONTACT_NAME, ios::in);

  char info[100];
  while(contact.getline(info, 100)) {
    char* name = strtok(info, (const char *)" ");
    char* age = strtok(NULL, (const char *)" ");
    char* sex = strtok(NULL, (const char *)" ");
    char* addr = strtok(NULL, (const char *)" ");

    Person p;
    p.name = name;
    p.age = atoi((const char *)age);
    p.sex = atoi((const char *)sex);
    p.addr = addr;
  
    ab->persons[ab->size++] = p;
  }
  contact.close();
}

/*
* 更新文本中联系人信息
*/
void updateContact(AddressBook *ab) {
  ofstream contact;
  contact.open((const char*)CONTACT_NAME, ios::out | ios::trunc);

  int i = 0;
  Person p;

  while(i < ab->size) {
    p = ab->persons[i++];
    contact << p.name << " " << p.age << " " << p.sex << " " << p.addr << endl;
  }
  contact.close();
}
inline void toBeContinued() {
  cout << "按Enter键继续..." << endl;
  cin.get();
  cin.get();
}

Person getPesonFromCli() {
  Person p;
  cout << "请输入联系人名称：" << endl;
  string name;
  cin >> name;
  p.name = name;

  cout << "请输入联系人年龄：" << endl;
  int age;
  cin >> age;
  p.age = age;

  int sex;
  do {
    cout << "请选择联系人性别：" << endl;
    cout << "男：1" << endl;
    cout << "女：2" << endl;
    cin >> sex;
    if (sex == 1 || sex == 2) break;
  } while (true);
  p.sex = sex;

  cout << "请输入联系人地址：" << endl;
  string addr;
  cin >> addr;
  p.addr = addr;

  return p;
}

void addPerson(AddressBook *ab) {
  cout << "***** 添加联系人 *****" << endl;
  ab->persons[ab->size] = getPesonFromCli();
  ab->size++;
  cout << "添加成功！" << endl;
  toBeContinued();
}

void printPerson(Person *p) {
  cout << "姓名：" << p->name << "\t";
  cout << "年龄：" << p->age << "\t";
  cout << "性别：" << (p->sex == 1 ? "男" : "女") << "\t";
  cout << "地址：" << p->addr << "\n";
}

void showPersons(AddressBook *ab) {
  for (int i = 0; i < ab->size; i++) {
    printPerson(&ab->persons[i]);
  }

  if (!ab->size) {
    cout << "暂无联系人信息！" << endl;
  }
  toBeContinued();
}

int isExist(AddressBook *ab, string name) {
  for (int i = 0; i < ab->size; i++) {
    if (ab->persons[i].name == name) return i;
  }
  return -1;
}

void deletePerson(AddressBook *ab) {
  cout << "***** 删除联系人 *****" << endl;
  cout << "请输入要删除的人名：" << endl;
  string name;
  cin >> name;

  int idx = isExist(ab, name);
  if (idx != -1) {
    // 删除, 做覆盖，ab->size--
    for (; idx < ab->size; idx++) {
      ab->persons[idx] = ab->persons[idx + 1];
    }
    ab->size--;
    cout << "删除 " << name << " 成功!" << endl;
  } else {
    cout << "查无此人" << endl;
  }
  toBeContinued();
}

void findPerson(AddressBook *ab) {
  cout << "***** 查找联系人 *****" << endl;
  cout << "请输入要查找的人名：" << endl;
  string name;
  cin >> name;

  int idx = isExist(ab, name);
  if (idx != -1) {
    printPerson(&ab->persons[idx]);
  } else {
    cout << "查无此人" << endl;
  }
  toBeContinued();
}

void editPerson(AddressBook *ab) {
  cout << "***** 修改联系人 *****" << endl;
  cout << "请输入要修改的人名：" << endl;
  string name;
  cin >> name;

  int idx = isExist(ab, name);
  if (idx != -1) {
    ab->persons[idx] = getPesonFromCli();
  } else {
    cout << "查无此人" << endl;
  }
  toBeContinued();
}

void clearAddressBook(AddressBook *ab) {
  cout << "***** 清空联系人 *****" << endl;
  ab->size = 0;
  cout << "清空联系人成功！" << endl;
  toBeContinued();
}

int main() {

  int select = 0;
  AddressBook ab;
  ab.size = 0;

  loadContact(&ab);

  while (true) {
    system("clear");
    showMenu();

    cin >> select;

    switch (select) {
      case 1:
        addPerson(&ab);
        updateContact(&ab);
        break;
      case 2:
        showPersons(&ab);
        break;
      case 3:
        deletePerson(&ab);
        updateContact(&ab);
        break;
      case 4:
        findPerson(&ab);
        break;
      case 5:
        editPerson(&ab);
        updateContact(&ab);
        break;
      case 6:
        clearAddressBook(&ab);
        updateContact(&ab);
        break;
      case 0:
        cout << "欢迎下次使用！" << endl;
        toBeContinued();
        return 0;
        break;
      default:
        break;
    }
  }

  return 0;
}