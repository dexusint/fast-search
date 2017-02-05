#include <cassert>
#include <iostream>
#include <random>
#include <future>
#include <thread>

#include <include/application.h>

using namespace std;

time_t Application::makeTimePoint(int year, int mon, int day, int hour, int min, int sec){
  struct std::tm t;
  t.tm_sec = sec;
  t.tm_min = min;
  t.tm_hour = hour;
  t.tm_mday = day;
  t.tm_mon = mon -1;
  t.tm_year = year - 1900;
  t.tm_isdst = -1;

  return std::mktime(&t);
}

void Application::fillCache(int cacheId, std::unordered_map<int, CacheData>& cache, int globalId) {
  auto pairIt = cache.find(cacheId);

  if(pairIt != cache.end()){
    auto& pair = cache[cacheId];
    pair.first.insert(globalId);
    pair.second.insert(globalId);

  } else {
    CacheData val = CacheData(unordered_set<int/*id*/>(), set<int/*id*/, Compare>(cmp));
    val.first.insert(globalId);
    val.second.insert(globalId);
    cache[cacheId] = val;
  }
}

void Application::genUsers(int numOfUsers, int birthYearBeg, int birthYearEnd, int regYearBeg, int regYearEnd, int cityIdBeg, int cityIdEnd) {
  const time_t begBirthDate = makeTimePoint(birthYearBeg, 1, 1, 0, 0, 0);
  const time_t endBirthDate = makeTimePoint(birthYearEnd, 12, 31, 23, 59, 59);
  const time_t begRegDate = makeTimePoint(regYearBeg, 1, 1, 0, 0, 0);
  const time_t endRegDate = makeTimePoint(regYearEnd, 12, 31, 23, 59, 59);

  knuth_b e;
  uniform_int_distribution<const time_t> uniformBirthDayGen(begBirthDate, endBirthDate);
  uniform_int_distribution<const time_t> uniformRegDayGen(begRegDate, endRegDate);
  uniform_int_distribution<const time_t> uniformGenderGen(0, 1);
  uniform_int_distribution<const time_t> uniformCityIdGen(cityIdBeg, cityIdEnd);

  int id = 0;

  for(id = 0; id < MAX_COUNT; ++id) {
    const auto randomBirthDate = uniformBirthDayGen(e);
    const auto randomRegDate = uniformRegDayGen(e);
    const auto randomGender = uniformGenderGen(e);
    const auto randomCityId = uniformCityIdGen(e);
    users.insert(std::pair<int, User>(id, User(id, randomBirthDate, randomGender, randomCityId, randomRegDate)));

    struct std::tm t = *localtime(&randomBirthDate);
    auto randomYear = t.tm_year;

    time_t  timeNow;
    time(&timeNow);

    struct std::tm t1 = *localtime(&timeNow);
    auto currYear = t1.tm_year;

    int age = currYear - randomYear;
    //cout << age << endl;

    fillCache(randomCityId, citiesCache, id);
    fillCache(randomGender, genderCache, id);
    fillCache(age, ageCache, id);
  }
  cout << endl;
}

void Application::executeQueryDecorator(int cityId, int gender, int age) {
  set<int, Compare> *base = nullptr;
  unordered_set<int/*id*/> *fistUSet = nullptr, *secondUSet = nullptr;

  if (cityId != -1){
    base = &get<1>(citiesCache.at(cityId));
    if(gender != -1)fistUSet = &get<0>(genderCache.at(gender));
    if(age != -1)secondUSet = &get<0>(ageCache.at(age));
  } else if(gender != -1) {
    base = &get<1>(genderCache.at(gender));
    if(cityId != -1)fistUSet = &get<0>(citiesCache.at(cityId));
    if(age != -1)secondUSet = &get<0>(ageCache.at(age));
  } else if(age != -1){
    base = &get<1>(ageCache.at(age));
    if(cityId != -1)fistUSet = &get<0>(citiesCache.at(cityId));
    if(gender != -1)secondUSet = &get<0>(genderCache.at(gender));
  } else {
    cout << "No values for processing..." << endl;
    return;
  }

  int counter = 0, outFromPrint = 0;

  auto system_start = chrono::system_clock::now();

  for (auto id : *base) {
    if(((fistUSet == nullptr) || (*fistUSet).find(id) != (*fistUSet).end()) &&
       ((secondUSet == nullptr) || (*secondUSet).find(id) != (*secondUSet).end()))
    {
      if (counter++ < NUM_OUTPUT_LINES) {
        auto user = users[id];
        cout  << "User id: " << user.id
              << " birthday: " << user.birthday
              << " gender: " << user.gender
              << " cityId: " << user.city_id
              << " reg date: " << user.time_reg << endl;
      } else {
        auto diff = chrono::system_clock::now() - system_start;
        auto micsec = chrono::duration_cast<chrono::microseconds>(diff);
        cout << "Time on request: " << micsec.count() << "e-6 s" << endl;
        cout << "Push any character and Enter to continue or -1 to break" << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        cin >> outFromPrint;
        if (outFromPrint == -1)break;
        system_start = chrono::system_clock::now();
        counter = 0;
      }
    }
  }
  if (counter != 0 && outFromPrint != -1) {
    auto diff = chrono::system_clock::now() - system_start;
    auto micsec = chrono::duration_cast<chrono::microseconds>(diff);
    cout << "Time on request: " << micsec.count() << "e-6 s" << endl;
  }

  cout << "End of output results" << endl;
}

void Application::executeQuery(int cityId, int gender, int age) {
  const auto& cityCacheData = citiesCache.at(cityId);
  const auto& citiesCacheDataSet = get<1>(cityCacheData);
  const auto& citiesCacheDataUnorderedSet = get<0>(cityCacheData);

  const auto& genderCacheData = genderCache.at(gender);
  const auto& genderCacheDataSet = get<1>(genderCacheData);
  const auto& genderCacheDataUnorderedSet = get<0>(genderCacheData);

  const auto& ageCacheData = ageCache.at(age);
  const auto& ageCacheDataSet = get<1>(ageCacheData);
  const auto& ageCacheDataUnorderedSet = get<0>(ageCacheData);

  int counter = 0, a = 0;

  auto system_start = chrono::system_clock::now();

  for (auto id : citiesCacheDataSet) {
    if(genderCacheDataUnorderedSet.find(id) != genderCacheDataUnorderedSet.end() &&
        ageCacheDataUnorderedSet.find(id) != ageCacheDataUnorderedSet.end())
    {
      if (counter++ < NUM_OUTPUT_LINES) {
        auto user = users[id];
        cout  << "User id: " << user.id
              << " birthday: " << user.birthday
              << " gender: " << user.gender
              << " cityId: " << user.city_id
              << " reg date: " << user.time_reg << endl;
      } else {
        auto diff = chrono::system_clock::now() - system_start;
        auto micsec = chrono::duration_cast<chrono::microseconds>(diff);
        cout << "Time on request: " << micsec.count() << "e-6 s" << endl;
        cout << "Push any character and Enter to continue or 0 to exit" << endl;
        cin >> a;
        if (a == 0)break;
        system_start = chrono::system_clock::now();
        counter = 0;
      }
    }
  }
  auto diff = chrono::system_clock::now() - system_start;
  auto micsec = chrono::duration_cast<chrono::microseconds>(diff);
  cout << "Time on request: " << micsec.count() << "e-6 s" << endl;

  cout << "End of output results" << endl;
}
