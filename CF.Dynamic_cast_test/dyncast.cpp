//
//  dyncast.cpp
//  CF.Dynamic_cast_test
//
//  Created by Alan Sampson on 3/29/21.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <typeinfo>
#include <chrono>
#include <stdexcept>
#include <random>
#include <array>
#include <span>
#include <algorithm>
#include <numeric>
#include <memory>
#include <cstdlib>

/**
 *  You can change the test settings/samples here
 */

auto constexpr SAMPLE_COUNT(1'250'000);
auto constexpr INSTANCE_COUNT(50);
auto constexpr REPEAT_COUNT(10);

/**
 *  Comment or Uncomment the defines below to change what test will occur
 */

//#define DYNAMIC_CAST
//#define TYPE_ID

enum TYPE_SELECT {
  DYNAMIC_CAST, TYPE_ID,
};

int dctest(TYPE_SELECT Typ);

class Parent_ {
public:
//  virtual ~Parent() = default;
  virtual ~Parent_() {
    std::cout << "Parent_::" << __func__ << std::endl;
  };
  bool parent;

public:
  virtual auto get_parent() -> decltype(parent) = 0;
};


class Child_A : public Parent_ {
public:
  int a_val = 0;

public:
  virtual ~Child_A() {
    std::cout << "Child_A::" << __func__ << std::endl;
  };

  auto get_parent() -> decltype(Parent_::parent) {
    return Parent_::parent;
  }
  auto get_val() -> decltype(a_val) {
    return a_val;
  }
};

class Child_B : public Parent_ {
public:
  float b_val = 0;

public:
  virtual ~Child_B() {
    std::cout << "Child_B::" << __func__ << std::endl;
  };

  auto get_parent() -> decltype(Parent_::parent) {
    return Parent_::parent;
  }
  auto get_val() -> decltype(b_val) {
    return b_val;
  }
};

class Child_C : public Parent_ {
public:
  double c_val = 0;

public:
  virtual ~Child_C() {
    std::cout << "Child_C::" << __func__ << std::endl;
  };

  auto get_parent() -> decltype(Parent_::parent) {
    return Parent_::parent;
  }
  auto get_val() -> decltype(c_val) {
    return c_val;
  }
};

class Child_D : public Parent_ {
public:
  char d_val = 0;

public:
  virtual ~Child_D() {
    std::cout << "Child_D::" << __func__ << std::endl;
  };

  auto get_parent() -> decltype(Parent_::parent) {
    return Parent_::parent;
  }
  auto get_val() -> decltype(d_val) {
    return d_val;
  }
};

//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  ================================================================================
/*
 *  MARK: main()
 */
int main(int argc, const char * argv[]) {
  std::cout << "CF.Dynamic_cast_test\n";
  std::cout << "C++ Version: " << __cplusplus << std::endl;

  try {
    dctest(TYPE_SELECT::DYNAMIC_CAST);

    dctest(TYPE_SELECT::TYPE_ID);
  }
  catch (std::out_of_range & ex) {
    std::cerr << ex.what() << std::endl;
  }

  return 0;
}

/*
 *  MARK: dctest()
 */
int dctest(TYPE_SELECT Typ) {
  using namespace std::chrono;

  std::random_device rdev;

  double final_average(0.0);
//  size_t * memory = new size_t[SAMPLE_COUNT];

//  std::array<size_t, SAMPLE_COUNT> memory;

  for (int LOOPS = 0; LOOPS < REPEAT_COUNT; ++LOOPS) {
    // loop entire program
    std::cout << '\n' << std::string(40, '=') << '\n'
              << "iteration" << std::setw(4) << LOOPS + 1
              << " of" << std::setw(5) << REPEAT_COUNT << '\n'
              << '\n';

    auto samples = std::make_unique<duration<size_t, std::nano>[]>(SAMPLE_COUNT);
//    auto * samples = memory;
//    std::array<size_t, SAMPLE_COUNT> samples;
    size_t sample(0ul);

//    // randomize seed
//    srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    if (Typ == TYPE_SELECT::DYNAMIC_CAST) {
//#ifdef DYNAMIC_CAST
      std::cout << "Running Test With DYNAMIC CASTING" << std::endl;
    }
//#endif  /* ifdef DYNAMIC_CAST */
//#ifdef TYPE_ID
    else {
      std::cout << "Running Test With TYPE ID CHECKING" << std::endl;
//#endif  /* ifdef TYPE_ID */
    }

//    Parent * classes[INSTANCE_COUNT];
    std::array<std::unique_ptr<Parent_>, INSTANCE_COUNT> classes;
    std::uniform_int_distribution<int> udis(0, 3);
    for (int ic = 0; ic < INSTANCE_COUNT; ++ic) {
      auto swtchtype = udis(rdev);
      switch(/*random() % 4*/ swtchtype) {
        case 0:
          classes[ic] = std::make_unique<Child_A>();
          break;

        case 1:
          classes[ic] = std::make_unique<Child_B>();
          break;

        case 2:
          classes[ic] = std::make_unique<Child_C>();
          break;

        case 3:
          classes[ic] = std::make_unique<Child_D>();
          break;

        default: {
            std::ostringstream os;
            os << "missing class type " << swtchtype;
            throw std::out_of_range(os.str());
          }
          break;
      }
    }

    int hit(0);
    int found (0);
    // record time start
    auto start = high_resolution_clock::now();
    auto end = start;
    while (sample < SAMPLE_COUNT) {
      std::uniform_int_distribution<int> udict(0, INSTANCE_COUNT - 1);
      auto classes_elmt = udict(rdev);
//      Parent * ii = classes[random() % INSTANCE_COUNT];
      auto prnt_ptr = classes[classes_elmt].get();

      start = high_resolution_clock::now();
//#ifdef TYPE_ID
      if (Typ == TYPE_SELECT::TYPE_ID) {
        if (typeid(*prnt_ptr) == typeid(Child_A)) {
          static_cast<Child_A*>(prnt_ptr)->a_val++;
          ++hit;
        }
      }
//#endif  /* ifdef TYPE_ID */
//#ifdef DYNAMIC_CAST
      else {
        Child_A * chldA_ptr = dynamic_cast<Child_A *>(prnt_ptr);
        if (chldA_ptr != nullptr) {
          chldA_ptr->a_val++;
//#ifndef TYPE_ID
          ++hit;
//#endif  /* ifndef TYPE_ID */
        }
      }
//#endif  /* ifdef DYNAMIC_CAST */
      end = high_resolution_clock::now(); // record time end
//
      // dump sample into memory
      auto diff = end - start;
      samples[sample++] = diff;
//        static_cast<size_t>(duration_cast<nanoseconds>(diff).count());
    }

    //  ....+....!....+....!....+....!....+....!....+....!....+....!
    std::cout << std::string(80, '~') << std::endl;

    double average = 0.0;
    auto memoryspn = std::span ( samples.get(), SAMPLE_COUNT );
    uint32_t sc(0);
    std::for_each(memoryspn.begin(), memoryspn.end(), [&sc, &average](auto & me) {
      if (sc++ < 16) {
        std::cout << "[" << duration_cast<nanoseconds>(me).count() << "] ";
      }
      average += duration_cast<nanoseconds>(me).count();
    });
//    for (int sc = 0; sc < SAMPLE_COUNT; ++sc) {
//      auto nsec = memory[sc];
//      if (sc < 16) {
//        std::cout << "[" << duration_cast<nanoseconds>(nsec).count() << "] ";
//      }
//      average += duration_cast<nanoseconds>(nsec).count();
//    }
    std::cout << "..." << std::endl;

    average /= SAMPLE_COUNT;

    std::cout << "Average: " << average << " nanoseconds" << std::endl;
    std::cout << "Child_A Classes Hit: " << hit << std::endl;

    for (int ic = 0; ic < INSTANCE_COUNT; ++ic) {
      if (dynamic_cast<Child_A *>(classes[ic].get())) {
        ++found;
      }
//      delete classes[ic];
    }

    std::cout << "Child_A Classes Found: " << found << std::endl;

    final_average += average;
    std::cout << '\n' << std::string(40, '=') << '\n'
              << "continuing master loop" << std::endl;
  }

//  delete [] memory;

  final_average /= REPEAT_COUNT;
  std::cout << "-----------------\nAll Tests Averaged: "
          << final_average << " nanoseconds"
          << std::endl;

  return 0;
}
