#include "options.h"
#include "filescanner.h"
#include "duplicatescanner.h"
#include "common.h"

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

using namespace testing;

class Bayan_Test : public ::testing::Test
{
protected:
  std::vector<std::vector<std::string>> Run(const std::vector<std::string>& a_arguments)
  {      
    std::vector<std::string> arguments {a_arguments};
    std::vector<char*> argv;
    for (const auto& arg : arguments) {
      argv.push_back((char*)arg.data());
    }
    argv.push_back(nullptr);

    Otus::OptionsParser optionsParser;
    auto options = optionsParser.GetOptions(argv.size() - 1, argv.data());

    Otus::FileScanner fileScanner(options.get().excludePaths, options.get().levelScannig, options.get().masks, options.get().minFileSize);
    auto groupPath = fileScanner.Scan(options.get().includePaths);

    Otus::Duplicatescanner dupScanner(options.get().blockSize, options.get().hash);
    auto duplicates = dupScanner.Scan(groupPath);

    std::vector<std::vector<std::string>> resultDuplicateFileNames;
    for (auto& dup : duplicates) {
      std::vector<std::string> fileNames;
      for (auto& path : dup) {
        fileNames.emplace_back(path.filename().string());
      }
      resultDuplicateFileNames.emplace_back(fileNames);
    }

    return resultDuplicateFileNames;
  }

};

TEST_F(Bayan_Test, Test_1_Simple) {   
  auto result = Run({"test_bayan", "-idata/test_1/"});
  ASSERT_EQ(result.size(), (std::size_t)2);
  ASSERT_EQ(result[0].size(), (std::size_t)2);
  ASSERT_EQ(result[1].size(), (std::size_t)2);
}

TEST_F(Bayan_Test, Test_1_Masks) {   
  auto result = Run({"test_bayan", "-idata/test_1/", "-m*.cpp"});
  ASSERT_EQ(result.size(), (std::size_t)1);
  ASSERT_EQ(result[0].size(), (std::size_t)2);
  ASSERT_THAT(result[0], ElementsAre( StrEq("hello_copy.cpp"), StrEq("hello.cpp") ));

  result = Run({"test_bayan", "-idata/test_1/", "-m*.txt"});
  ASSERT_EQ(result.size(), (std::size_t)1);
  ASSERT_EQ(result[0].size(), (std::size_t)2);
  ASSERT_THAT(result[0], ElementsAre( StrEq("file2.txt"), StrEq("file1.txt") ));
}

TEST_F(Bayan_Test, Test_1_Size) {   
  auto result = Run({"test_bayan", "-idata/test_1/", "-s100"});
  ASSERT_EQ(result.size(), (std::size_t)1);
  ASSERT_EQ(result[0].size(), (std::size_t)2);
  ASSERT_THAT(result[0], ElementsAre( StrEq("hello_copy.cpp"), StrEq("hello.cpp") ));
}

TEST_F(Bayan_Test, Test_2_Level) {   
  auto result = Run({"test_bayan", "-idata/test_2", "-l1"});
  ASSERT_EQ(result.size(), (std::size_t)2);
  ASSERT_EQ(result[0].size(), (std::size_t)2);
  ASSERT_EQ(result[1].size(), (std::size_t)2);
  // EXPECT_THAT(result[0], ElementsAre( StrEq("hello_copy.cpp"), StrEq("hello.cpp") ));
  // EXPECT_THAT(result[1], ElementsAre( StrEq("just_single_file_copy.doc"), StrEq("just_single_file.doc")));
}

TEST_F(Bayan_Test, Test_2_Exclude) {   
  auto result = Run({"test_bayan", "-idata/test_2/", "-edata/test_2/level_1/", "-l1" });
  ASSERT_EQ(result.size(), (std::size_t)1);
  ASSERT_EQ(result[0].size(), (std::size_t)2);
  ASSERT_THAT(result[0], ElementsAre( StrEq("hello_copy.cpp"), StrEq("hello.cpp") ));
}

TEST_F(Bayan_Test, Test_3_Multi) {   
  auto result = Run({"test_bayan", "-idata/test_3/", "-l2"});
  ASSERT_EQ(result.size(), (std::size_t)1);
  ASSERT_EQ(result[0].size(), (std::size_t)3);
  // EXPECT_THAT(result[0], ElementsAre( StrEq("file3.txt"), StrEq("file1.txt"), StrEq("file2.txt") ));
}