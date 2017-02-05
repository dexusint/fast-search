

class User {
public:
 User() = default;
 User(int id, time_t birthday, int gender, int city_id, time_t time_reg){
   this->id = id;
   this->birthday = birthday;
   this->gender = gender;
   this->city_id = city_id;
   this->time_reg = time_reg;
 }

 int id; 
 time_t birthday; // дата рождения
 int gender;  // пол
 int city_id;  // место проживания
 time_t time_reg; // дата регистрации
};
