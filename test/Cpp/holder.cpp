#include <catch.hpp>
#include <IbWinCpp/Cpp/holder.hpp>

static int count = 0;
struct counter {
    counter() {
        count++;
    }
    counter(int new_count) {
        count = new_count;
    }
    ~counter() {
        count--;
    }

    void inc() {
        count++;
    }
    void dec() {
        count--;
    }
};

TEST_CASE("Holder") {
    count = 0;
    {
        ib::Holder<counter> holder;
        CHECK(count == 0);

        holder.create();
        CHECK(count == 1);
        holder.create();
        CHECK(count == 2);

        holder.recreate();
        CHECK(count == 2);

        holder->inc();
        CHECK(count == 3);
        (*holder).dec();
        CHECK(count == 2);

        holder.destroy();
        CHECK(count == 1);
        holder.destroy();
        CHECK(count == 0);
    }
    CHECK(count == 0);

    count = 0;
    SECTION("create with arguments") {
        ib::Holder<counter> holder(123);
        CHECK(count == 123);

        holder.create(456);
        CHECK(count == 456);

        holder.recreate(789);
        CHECK(count == 789);
    }

    count = 0;
    SECTION("create with default ctor") {
        ib::Holder<counter> holder(ib::Holder<void>::Default);
        CHECK(count == 1);
    }
}

TEST_CASE("HolderB") {
    count = 0;
    {
        ib::HolderB<counter> holder;
        CHECK(count == 0);
        CHECK(!holder.created());

        holder.recreate();
        CHECK(count == 1);
        CHECK(holder.created());

        holder.destroy();
        CHECK(count == 0);
        CHECK(!holder.created());

        holder.destroy();
        CHECK(count == 0);
        CHECK(!holder.created());

        holder.create();
        CHECK(count == 1);
        CHECK(holder.created());

        holder->inc();
        CHECK(count == 2);
        (*holder).dec();
        CHECK(count == 1);

        holder.destroy();
        CHECK(count == 0);
        CHECK(!holder.created());
    }
    CHECK(count == 0);

    count = 0;
    SECTION("create with arguments") {
        ib::HolderB<counter> holder(123);
        CHECK(count == 123);
        CHECK(holder.created());

        holder.recreate(456);
        CHECK(count == 456);
    }

    count = 0;
    SECTION("create with default ctor") {
        ib::HolderB<counter> holder(ib::Holder<void>::Default);
        CHECK(count == 1);
        CHECK(holder.created());
    }
}