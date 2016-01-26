#include <vector>
#include <algorithm>
#include <climits>


class Coordinate {

private:
  
  //  const std::vector<unsigned int> dimensional_maximum;

  const std::vector<unsigned int> value;

public:

  Coordinate() = delete;

  Coordinate(const std::vector<unsigned int> v) : value(v) {
    // This space intentionally left blank
  }

  bool operator <(const Coordinate& that) {
    return std::lexicographical_compare(value.begin(), value.end(), 
                                        that.value.begin(),  that.value.end());
  }

  bool operator ==(const Coordinate& that) {
    return std::equal(value.begin(), value.end(), 
                      that.value.begin(), that.value.end());
  }

  friend std::ostream& operator<<(std::ostream& os, const Coordinate& c);

};

 std::ostream& operator<<(std::ostream& os, const Coordinate& c)
 {

   unsigned int size = c.value.size();
   unsigned int lastIndex = size - 1;

   os << '(';
   for (unsigned int index=0; index<lastIndex; index+=1) {
     os << c.value[index] << ',';
   }
   os << c.value[lastIndex] << ')';

   return os;
 }


class MyIterator3 : public std::iterator<std::input_iterator_tag, Coordinate>
{

private:

  const std::vector<unsigned int> dimensional_maximum;

  void nextValue() {

    bool changed = false;

    for (unsigned int dimension=0, num_dimensions=value.size(); dimension < num_dimensions; dimension += 1) {
      if (value[dimension] + 1 <= dimensional_maximum[dimension]) {
	value[dimension] += 1;
	changed = true;
	break;
      } else {
	value[dimension] = 0;
      }
    }

    if (! changed) {
      for (unsigned int dimension=0, num_dimensions=value.size(); dimension < num_dimensions; dimension += 1) {
      	value[dimension] = UINT_MAX;
      }
    }
  }

  std::vector<unsigned int> value;

public:
  MyIterator3(std::vector<unsigned int> maxima, std::vector<unsigned int> v) : dimensional_maximum(maxima), value(v) {}
  MyIterator3(const MyIterator3& mit) : dimensional_maximum(mit.dimensional_maximum), value(mit.value) {}
  MyIterator3& operator++() {nextValue();return *this;}
  MyIterator3 operator++(int) {MyIterator3 tmp(*this); operator++(); return tmp;}
  bool operator==(const MyIterator3& rhs) {
    return std::equal(value.begin(), value.end(), 
                      rhs.value.begin(), rhs.value.end());

  }
  bool operator!=(const MyIterator3& rhs) {return !(*this==rhs);}
  Coordinate operator*() {return Coordinate(value);}
};


class DEF3 {

  const std::vector<unsigned int> dimensional_maximum;


public:

  DEF3(std::initializer_list<unsigned int> maxima) : dimensional_maximum(maxima) {}

  MyIterator3 begin() {
    return MyIterator3(dimensional_maximum, std::vector<unsigned int>(dimensional_maximum.size(), 0));
  }

  MyIterator3 end() {
    return MyIterator3(dimensional_maximum, std::vector<unsigned int>(dimensional_maximum.size(), UINT_MAX));
  }

};



class MyIterator2 : public std::iterator<std::input_iterator_tag, int>
{
  int p;
public:
  MyIterator2(int x) :p(x) {}
  MyIterator2(const MyIterator2& mit) : p(mit.p) {}
  MyIterator2& operator++() {++p;return *this;}
  MyIterator2 operator++(int) {MyIterator2 tmp(*this); operator++(); return tmp;}
  bool operator==(const MyIterator2& rhs) {return p==rhs.p;}
  bool operator!=(const MyIterator2& rhs) {return p!=rhs.p;}
  int& operator*() {return p;}
};


class DEF2 {


public:

  MyIterator2 begin() {
    return MyIterator2(0);
  }

  MyIterator2 end() {
    return MyIterator2(5);
  }

};



class MyIterator : public std::iterator<std::input_iterator_tag, int>
{
  int* p;
public:
  MyIterator(int* x) :p(x) {}
  MyIterator(const MyIterator& mit) : p(mit.p) {}
  MyIterator& operator++() {++p;return *this;}
  MyIterator operator++(int) {MyIterator tmp(*this); operator++(); return tmp;}
  bool operator==(const MyIterator& rhs) {return p==rhs.p;}
  bool operator!=(const MyIterator& rhs) {return p!=rhs.p;}
  int& operator*() {return *p;}
};


class DEF {

  int b = 0;

  int e = 10;

  int *numbers;

public:

  DEF(int *n) : numbers(n) {

  }

  MyIterator begin() {
    return MyIterator(numbers);
  }

  MyIterator end() {
    return MyIterator(numbers + 5);
  }

};




class ABC {

private:

  const std::vector<unsigned int> dimensional_maximum;

public:

  ABC(std::initializer_list<unsigned int> maxima) : dimensional_maximum(maxima) {
    // This space intentionally left blank
  }

  

};

