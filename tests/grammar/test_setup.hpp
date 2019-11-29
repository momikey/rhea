#ifndef RHEA_GRAMMAR_TEST_SETUP_HPP
#define RHEA_GRAMMAR_TEST_SETUP_HPP

struct fixture
{
    fixture() = default;
    ~fixture() = default;

    void setup() {}
    void teardown() {}
};

// Wrapper for parser + EOR
template<typename T>
struct simple_parser : tao::pegtl::seq<
    T, tao::pegtl::eolf> {};

#endif /* RHEA_GRAMMAR_TEST_SETUP_HPP */