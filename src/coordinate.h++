#include <vector>
#include <algorithm>


class MyIterator3 : public std::iterator<std::input_iterator_tag, int>
{

private:

  const std::vector<unsigned int> dimensional_maximum;

  

  int p;
public:
  MyIterator3(int x, std::initializer_list<unsigned int> maxima) : dimensional_maximum(maxima), p(x) {}
  MyIterator3(const MyIterator3& mit) : p(mit.p) {}
  MyIterator3& operator++() {++p;return *this;}
  MyIterator3 operator++(int) {MyIterator3 tmp(*this); operator++(); return tmp;}
  bool operator==(const MyIterator3& rhs) {return p==rhs.p;}
  bool operator!=(const MyIterator3& rhs) {return p!=rhs.p;}
  int& operator*() {return p;}
};


class DEF3 {


public:

  MyIterator3 begin() {
    return MyIterator3(0);
  }

  MyIterator3 end() {
    return MyIterator3(5);
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


class Coordinate {

private:
  
  const std::vector<unsigned int> dimensional_maximum;

  const std::vector<unsigned int> value;

  std::vector<unsigned int> nextValue() {

    std::vector<unsigned int> result(value);

    for (unsigned int dimension=0, num_dimensions=value.size(); dimension < num_dimensions; dimension += 1) {
      if (result[dimension] + 1 <= dimensional_maximum[dimension]) {
	result[dimension] += 1;
	break;
      }
    }

    return result;

  }

  Coordinate(const std::vector<unsigned int> maxima, const std::vector<unsigned int> v) : dimensional_maximum(maxima), value(v) { }

public:

  Coordinate() = delete;

  Coordinate(std::initializer_list<unsigned int> maxima) : dimensional_maximum(maxima), value(maxima.size(), 0) {
    //  Coordinate(const std::vector<unsigned int> maxima) : dimensional_maximum(maxima), value(maxima.size(), 0) {
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


  Coordinate next() {
    return Coordinate(this->dimensional_maximum, this->nextValue());
  }

  Coordinate final() {
    return Coordinate(this->dimensional_maximum, this->dimensional_maximum);
  }

};
