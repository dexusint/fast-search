#include <iostream>
#include <cstddef>
#include <limits.h>

#include "../include/application.h"

using namespace std;

int main(void)
{
  Application app;

  int menuItem = -1;

  while (true) {
    cout <<
R"(Select menu item:
1. Generate user database
2. Execute query
0. Exit)" << endl;

    cin >> menuItem;
    while (!cin)
    {
      cin.clear();
      cin.ignore(10000, '\n');
      cin >> menuItem;
    }

    switch(menuItem) {
      case(1): {
        app.genUsers(app.MAX_COUNT);
        cout << "Ready 1" << endl;
        break;
      }
      case(2): {
        time_t timeNow;
        time(&timeNow);
        struct std::tm t1 = *localtime(&timeNow);
        auto currYear = t1.tm_year + 1900;
        auto ageMin = currYear - app.getBirthYearMax();
        auto ageMax = currYear - app.getBirthYearMin();
        int age = 0, cityId = 0, gender = 0;

        cout << "Please input age["<< ageMin << "; " << ageMax << "], "
                                   << "gender[0; 1] and cityId[" << app.getCityIdMin() << "; " << app.getCityIdMax() << "]" << endl;

        do {
          cout << "Input age or -1 to skip parameter: " << endl;
          cin.clear();
          cin.ignore(10000, '\n');
          cin >> age;
          if(age == -1)break;
        } while ((ageMin > age) || (age > ageMax));

        do {
          cout << "Input gender -1 to skip parameter: " << endl;
          cin.clear();
          cin.ignore(10000, '\n');
          cin >> gender;
          if(gender == -1)break;
        } while ((0 > gender) || (gender > 1));

        do {
          cout << "Input cityId -1 to skip parameter: " << endl;
          cin.clear();
          cin.ignore(10000, '\n');
          cin >> cityId;
          if(cityId == -1)break;
        } while ((app.getCityIdMin() > cityId) || (cityId > app.getCityIdMax()));

        //app.executeQuery(cityId, gender, age);
        app.executeQueryDecorator(cityId, gender, age);
        cout << "Ready 2" << endl;
        break;
      }
      case(0): {
        cout << "Good bye!";
        return 0;
      }
    }

    menuItem = -1;
  }

  return EXIT_SUCCESS;
}
