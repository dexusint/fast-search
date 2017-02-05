#ifndef APPLICATION_H
#define APPLICATION_H

#include <cstddef>
#include <memory>
#include <ctime>
#include <unordered_map>
#include <unordered_set>
#include <set>

#include "user.h"

class Compare {
public:
  Compare(): users(nullptr){ }

  Compare(const std::unordered_map<int, User>* users){
    this->users = users;
  }

  bool operator()(const int& left, const int& right) const {
    return (*users).at(left).time_reg < (*users).at(right).time_reg ? true : false;
  }


private:
    const std::unordered_map<int, User>* users;
};

class Application {
public:
  enum Params {BIRTH_YEAR_MIN = 1975, BIRTH_YEAR_MAX = 1990, REG_YEAR_MIN = 2006, REG_YEAR_MAX = 2016, CITY_ID_MIN = 0, CITY_ID_MAX = 100};
  Application():cmp(&users){ }
public:
  typedef std::pair<std::unordered_set<int/*id*/>, std::set<int/*id*/, Compare>> CacheData;
  void genUsers(int numOfUsers, int birthYearBeg = BIRTH_YEAR_MIN,
                                int birthYearEnd = BIRTH_YEAR_MAX,
                                int regYearBeg = REG_YEAR_MIN,
                                int regYearEnd = REG_YEAR_MAX,
                                int cityIdBeg = CITY_ID_MIN,
                                int cityIdEnd = CITY_ID_MAX);
  void executeQuery(int cityId, int gender, int age);
  void executeQueryDecorator(int cityId, int gender, int age);

  const int MAX_COUNT = 1e7;
  const int NUM_OUTPUT_LINES = 100;

  void setBirthYearMin(int birthYearMin) {
    if(birthYearMin != this->birthYearMin)this->birthYearMin = birthYearMin;
  }

  int getBirthYearMin() {
    return birthYearMin;
  }

  void setBirthYearMax(int birthYearMax) {
    if(birthYearMax != this->birthYearMax)this->birthYearMax = birthYearMax;
  }

  int getBirthYearMax() {
    return birthYearMax;
  }

  void setCityIdMin(int cityIdMin) {
    if(cityIdMin != this->cityIdMin)this->cityIdMin = cityIdMin;
  }

  int getCityIdMin() {
    return cityIdMin;
  }

  void setCityIdMax(int cityIdMax) {
    if(cityIdMax != this->cityIdMax)this->cityIdMax = cityIdMax;
  }

  int getCityIdMax() {
    return cityIdMax;
  }


private:
  time_t makeTimePoint(int year, int mon, int day, int hour, int min, int sec=0);
  void fillCache(int cacheId, std::unordered_map<int/*cityId*/, CacheData>& cache, int globalId);
  std::unordered_map<int/*id*/, User> users;
  Compare cmp;
  std::unordered_map<int/*cityId*/, CacheData> citiesCache;
  std::unordered_map<int/*gender*/, CacheData> genderCache;
  std::unordered_map<int/*age*/, CacheData> ageCache;
  int birthYearMin = BIRTH_YEAR_MIN;
  int birthYearMax = BIRTH_YEAR_MAX;
  int cityIdMin = CITY_ID_MIN;
  int cityIdMax = CITY_ID_MAX;
};

#endif//APPLICATION_H
