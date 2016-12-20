// Copyright (c) 2015-2016 Hypha
#include <gmock/gmock.h>

#include "hyphawebmanager.h"

class core_test : public testing::Test {
 public:
};

TEST_F(core_test, CheckSomeResults) {
    HyphaWebManager hyphaWebManager;

    hyphaWebManager.handleConfig("p", "4242");
    ASSERT_TRUE(hyphaWebManager.config().getInt("port") == 4242);

    hyphaWebManager.handleConfig("f", "config.xml");
    ASSERT_TRUE(hyphaWebManager.config().getString("config-file") == "config.xml");
}
