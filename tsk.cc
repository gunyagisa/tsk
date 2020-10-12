#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>

using namespace nlohmann;

void show_all();
void write_json();
void usage();
void remove();

int main(int argc, char const* argv[])
{
  printf("Hello world\n");

  if (argc < 2) {
    usage();
    exit(1);
  }

  if (strncmp(argv[1], "ls", 2) == 0) {
    show_all();
  } else if (strncmp(argv[1], "add", 3) == 0) {
    write_json();
  } else if (strncmp(argv[1], "rm", 2) == 0) {
    remove();
  } else {
    switch (*argv[1]) {
      case 'q':
        printf("Good bye!\n");
        return 0;
        break;
    } } return 0;
}

void usage()
{
}

void show_all()
{
  std::ifstream i("content.json"); 

  json j;
  i >> j;

  std::sort(j.begin(), j.end(), [](json &i, json &k) {
      if (i["expire"]["month"].get<int>() == k["expire"]["month"].get<int>()) {
        return i["expire"]["day"].get<int>() < k["expire"]["day"].get<int>();
      } else {
        return i["expire"]["month"] < k["expire"]["month"];
      }
      });

  for (auto itr = j.begin(); itr != j.end(); ++itr) {
    json tmp = *itr;
    printf("%ld | ", std::distance(j.begin(), itr));
    printf("%02d/%02d\t", tmp["expire"]["month"].get<int>(), tmp["expire"]["day"].get<int>());
    printf("| %s", tmp["content"].get<std::string>().c_str());
  }
}

void write_json()
{
  std::ifstream i("content.json");
  json array;
  try {
    i >> array;
  } catch (json::exception) {
    array.empty();
  };

  for (;;) {
    printf("expire date: ");
    int month, day;
    char input[125];
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%d %d", &month, &day);
    printf("content: ");
    char buf[125];
    fgets(buf, sizeof(buf), stdin);
    auto j = json({
        {"expire", {
        {"month", month},
        {"day", day},
        }},
        {"content", std::string(buf)}
        });
    array.push_back(j);

    printf("continue? > ");
    char tmp[2];
    fgets(tmp, 2, stdin);
    if (tmp[0] == 'q') {
      break;
    } else {
      continue;
    }
  }

fin:

  std::sort(array.begin(), array.end(), [](json &i, json &k) {
      if (i["expire"]["month"].get<int>() == k["expire"]["month"].get<int>()) {
        return i["expire"]["day"].get<int>() < k["expire"]["day"].get<int>();
      } else {
        return i["expire"]["month"] < k["expire"]["month"];
      }
      });

  std::ofstream o("content.json");
  o << array;
  o.close();
}

void remove()
{
  std::ifstream i("content.json");
  json array;
  
  try {
    i >> array;
  } catch (json::exception) {
    printf("There are nothing or content.json file is broken\n");
    exit(1);
  };

  show_all();

  char buf[16];
  int index;
  printf("which index to remove? > ");
  std::cin >> index;

  array.erase(array.begin() + index);

  std::ofstream o("content.json");
  o << array;
  o.close();
}


