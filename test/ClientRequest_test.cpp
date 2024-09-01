/* #############################################################################

               """          ClientRequest_test.cpp
        -\-    _|__
         |\___/  . \        Created on 07 May. 2024 at 11:13
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "ClientRequest_test.hpp"
#include "ClientRequest.hpp"
#include "ClientRequestTest.hpp"
#include "HttpError.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "StringUtils.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <cstddef>
#include <exception>
#include <string>

class ClientRequestTestSuite : public ::testing::Test {
    protected:
        ClientRequestTestSuite() { info.disable(); }

        ~ClientRequestTestSuite() { info.enable(); }
};

/*
 * Can access private elements of ClientRequest.
 * LSP is wrong
 * */
TEST(ClientRequestTestSuite, ParseMethodTestExpectedOK) {
    ClientRequest test(0, "");

    EXPECT_EQ(test.parse_method("GET", 3), GET);
    EXPECT_EQ(test.parse_method("POST", 4), POST);
    EXPECT_EQ(test.parse_method("DELETE", 6), DELETE);
    EXPECT_EQ(test.parse_method("GET /var/srcs HTTP/1.1", 3), GET);
    EXPECT_EQ(test.parse_method("POST /var/srcs HTTP/1.1", 4), POST);
    EXPECT_EQ(test.parse_method("DELETE /var/srcs HTTP/1.1", 6), DELETE);
}

TEST(ClientRequestTestSuite, ParseMethodTestExpectedFail) {
    ClientRequest test(0, "");

    EXPECT_THROW(
        {
            try {
                test.parse_method("(&)", 3);
            } catch (HttpError &e) {
                EXPECT_EQ(e.get_code(), NotImplemented);
                throw;
            } catch (std::exception &e) {
                throw;
            }
        },
        HttpError
    );
    EXPECT_THROW(
        {
            try {
                test.parse_method("DELETED", 7);
            } catch (HttpError &e) {
                EXPECT_EQ(e.get_code(), NotImplemented);
                throw;
            } catch (std::exception &e) {
                throw;
            }
        },
        HttpError
    );
    EXPECT_THROW(
        {
            try {
                test.parse_method("G", 3);
            } catch (HttpError &e) {
                EXPECT_EQ(e.get_code(), NotImplemented);
                throw;
            } catch (std::exception &e) {
                throw;
            }
        },
        HttpError
    );
    EXPECT_THROW(
        {
            try {
                test.parse_method("", 0);
            } catch (HttpError &e) {
                EXPECT_EQ(e.get_code(), NotImplemented);
                throw;
            } catch (std::exception &e) {
                throw;
            }
        },
        HttpError
    );
    EXPECT_THROW(
        {
            try {
                test.parse_method("GET /var/srcs HTTP/1.1", 4);
            } catch (HttpError &e) {
                EXPECT_EQ(e.get_code(), NotImplemented);
                throw;
            } catch (std::exception &e) {
                throw;
            }
        },
        HttpError
    );
    EXPECT_THROW(
        {
            try {
                test.parse_method("DELETE /var/srcs HTTP/1.1", 2);
            } catch (HttpError &e) {
                EXPECT_EQ(e.get_code(), NotImplemented);
                throw;
            } catch (std::exception &e) {
                throw;
            }
        },
        HttpError
    );
    EXPECT_THROW(
        {
            try {
                test.parse_method("thismethodiswaytoolong", 22);
            } catch (HttpError &e) {
                EXPECT_EQ(e.get_code(), NotImplemented);
                throw;
            } catch (std::exception &e) {
                throw;
            }
        },
        HttpError
    );
}

TEST_P(ClientRequestTestTarget, ParseTargetTest) {
    std::string line = "METHOD ";
    size_t      pos  = 6;
    std::string params[3];

    t_test_target tmp = GetParam();

    params[0] = tmp.c1;
    params[1] = tmp.c2;
    params[2] = tmp.c3;
    line     += params[0];
    if (params[1] == "BadRequest") {
        info.disable();
        EXPECT_THROW(
            {
                try {
                    test.parse_target(sanitize_HTTP_string(line), pos);
                } catch (HttpError &e) {
                    EXPECT_EQ(e.get_code(), BadRequest);
                    throw;
                } catch (std::exception &e) {
                    throw;
                }
            },
            HttpError
        );
        info.enable();
    } else if (params[1] == "URITooLong") {
        info.disable();
        EXPECT_THROW(
            {
                try {
                    test.parse_target(sanitize_HTTP_string(line), pos);
                } catch (HttpError &e) {
                    EXPECT_EQ(e.get_code(), URITooLong);
                    throw;
                } catch (std::exception &e) {
                    throw;
                }
            },
            HttpError
        );
        info.enable();
    } else if (params[1] == "MovedPermanently") {
        info.disable();
        EXPECT_THROW(
            {
                try {
                    test.parse_target(sanitize_HTTP_string(line), pos);
                } catch (HttpError &e) {
                    EXPECT_EQ(e.get_code(), MovedPermanently);
                    EXPECT_EQ(e.what(), params[2]);
                    throw;
                } catch (std::exception &e) {
                    throw;
                }
            },
            HttpError
        );
        info.enable();
    } else {
        EXPECT_NO_THROW(test.parse_target(sanitize_HTTP_string(line), pos));
        EXPECT_EQ(test._target, params[1]);
        if (params[2] != "")
            EXPECT_EQ(test._header["Host"], params[2]);
        else
            EXPECT_EQ(test._header.find("Host"), test._header.end());
    }
}

INSTANTIATE_TEST_SUITE_P(
    ClientRequestTargetSuite,
    ClientRequestTestTarget,
    ::testing::ValuesIn(ClientRequestTargetSuiteValues),
    [](const testing::TestParamInfo< t_test_target > &info) {
        // Can use info.param here to generate the test suffix
        std::string name = info.param.name;
        return name;
    }
);

TEST(ClientRequestTestSuite, ParseHeaderLineTestHost) {
    std::string   header   = "Host: www.example.com";
    std::string   header_2 = "Host: www.coucou.com";
    ClientRequest test(0, "");

    EXPECT_NO_THROW(test.parse_header_line(header, 0, header.length()));
    EXPECT_EQ(test._header["Host"], "www.example.com");
    info.disable();
    EXPECT_THROW(
        {
            try {
                test.parse_header_line(header_2, 0, header_2.length());
            } catch (HttpError &e) {
                EXPECT_EQ(e.get_code(), BadRequest);
                throw;
            } catch (std::exception &e) {
                throw;
            }
        },
        HttpError
    );
    info.enable();
    EXPECT_EQ(test._header["Host"], "www.example.com");
}

TEST_P(ClientRequestTestParseHeader, ParseHeaderLineTest) {
    t_test_target tmp   = GetParam();
    size_t        begin = 0;
    size_t        end   = tmp.c1.length();

    if (tmp.c3 == "BadRequest") {
        ASSERT_THROW(
            {
                info.disable();
                try {
                    test.parse_header_line(sanitize_HTTP_string(tmp.c1), begin, end);
                    info.enable();
                } catch (HttpError &e) {
                    EXPECT_EQ(e.get_code(), BadRequest);
                    info.enable();
                    throw;
                } catch (std::exception &e) {
                    info.enable();
                    throw;
                }
            },
            HttpError
        );
        EXPECT_EQ(test._header.find(tmp.c2), test._header.end());
    } else {
        ASSERT_NO_THROW(test.parse_header_line(sanitize_HTTP_string(tmp.c1), begin, end));
        EXPECT_NE(test._header.find(tmp.c2), test._header.end());
        EXPECT_EQ(test._header[tmp.c2], tmp.c3);
    }
}

INSTANTIATE_TEST_SUITE_P(
    ClientRequestParseHeaderLineSuite,
    ClientRequestTestParseHeader,
    ::testing::ValuesIn(ClientRequestParseHeaderLineSuiteValues),
    [](const testing::TestParamInfo< t_test_target > &info) {
        // Can use info.param here to generate the test suffix
        std::string name = info.param.name;
        return name;
    }
);

TEST_P(ClientRequestTestInitHeader, InitHeaderTest) {
    t_test_messages tmp = GetParam();
    std::string     a   = sanitize_HTTP_string(tmp.request);

    test.parse_request_line(a);
    if (tmp.error == BadRequest) {
        info.disable();
        test.parse_headers(a);
        EXPECT_EQ(test.get_status(), BadRequest);
        info.enable();
    } else {
        ASSERT_NO_THROW(test.parse_headers(a));
        EXPECT_THAT(test._header, ::testing::ContainerEq(tmp.headers));
        EXPECT_EQ(tmp.headers.size(), test._header.size());
    }
}

INSTANTIATE_TEST_SUITE_P(
    ClientRequestTestInitHeaderSuite,
    ClientRequestTestInitHeader,
    ::testing::ValuesIn(ClientRequestTestData),
    [](const testing::TestParamInfo< t_test_messages > &info) {
        // Can use info.param here to generate the test suffix
        std::string name = info.param.name;
        return name;
    }
);

TEST(ClientRequestTestSuite, decode_target) {
    ClientRequest test(0, "");

    // with nothing
    test._target = "Coucou je suis heureux";
    test.decode_target();
    EXPECT_EQ(test._target, "Coucou je suis heureux");
    // with something
    test._target = "Coucou%20je%20suis%20heureux";
    test.decode_target();
    EXPECT_EQ(test._target, "Coucou je suis heureux");
    // with only one thing
    test._target = "%20";
    test.decode_target();
    EXPECT_EQ(test._target, " ");
    // with everything
    test._target = "%43%6F%75%63%6F%75%20%6A%65%20%73%75%69%73%20%68%65%75%72%65%75%78";
    test.decode_target();
    EXPECT_EQ(test._target, "Coucou je suis heureux");
}

TEST(ClientRequestTestSuite, decode_target_loop) {
    ClientRequest test(0, "");
    test._target = "coucou%2520super";
    test.decode_target();
    EXPECT_EQ(test._target, "coucou%20super");
}

/* TEST(ClientRequestTestSuite, parse_parameters) {
    ClientRequest test(0);

    // with nothing
    test._target = "/coucou";
    test.parse_parameters();
    ASSERT_EQ(test._parameters.size(), (size_t) 0);
    EXPECT_EQ(test._target, "/coucou");
    // with something
    test._target = "/coucou?je=suis";
    test.parse_parameters();
    ASSERT_EQ(test._parameters.size(), (size_t) 1);
    EXPECT_EQ(test._parameters["je"], "suis");
    EXPECT_EQ(test._target, "/coucou");
    // with more something
    test._parameters.clear();
    test._target = "/coucou?je=suis&heureux=ahah";
    test.parse_parameters();
    ASSERT_EQ(test._parameters.size(), (size_t) 2);
    EXPECT_EQ(test._parameters["je"], "suis");
    EXPECT_EQ(test._parameters["heureux"], "ahah");
    EXPECT_EQ(test._target, "/coucou");
    // with something empty
    test._parameters.clear();
    test._target = "/coucou?je&heureux=ahah";
    test.parse_parameters();
    ASSERT_EQ(test._parameters.size(), (size_t) 2);
    EXPECT_EQ(test._parameters["je"], "");
    EXPECT_EQ(test._parameters["heureux"], "ahah");
    EXPECT_EQ(test._target, "/coucou");
    // with nothing empty
    test._parameters.clear();
    test._target = "/coucou?";
    test.parse_parameters();
    ASSERT_EQ(test._parameters.size(), (size_t) 0);
    EXPECT_EQ(test._target, "/coucou");
} */

TEST(ClientRequestTestSuite, parse_parameters) {
    ClientRequest test(0, "");
    std::string   param;

    // with nothing
    param        = "";
    test._target = "/coucou" + param;
    test.parse_parameters();
    EXPECT_EQ(test._target, "/coucou");
    EXPECT_EQ(test._query_string, param);
    // with something
    param        = "je=suis";
    test._target = "/coucou?" + param;
    test.parse_parameters();
    EXPECT_EQ(test._target, "/coucou");
    EXPECT_EQ(test._query_string, param);
    // with more something
    param        = "je=suis&heureux=coucou";
    test._target = "/coucou?" + param;
    test.parse_parameters();
    EXPECT_EQ(test._target, "/coucou");
    EXPECT_EQ(test._query_string, param);
    // with nothing empty
    test._target = "/coucou?";
    test.parse_parameters();
    EXPECT_EQ(test._target, "/coucou");
    EXPECT_EQ(test._query_string, "");
}

TEST(ClientRequestTestSuite, parse_port) {
    ClientRequest test(0, "");

    EXPECT_THROW(
        {
            try {
                test.parse_port();
            } catch (HttpError &e) {
                EXPECT_EQ(e.get_code(), BadRequest);
                throw;
            } catch (std::exception &e) {
                throw;
            }
        },
        HttpError
    );
    test._header["Host"] = "";
    std::string &host    = test._header["Host"];
    host                 = "coucou";
    test.parse_port();
    EXPECT_EQ(test._port, 80);
    EXPECT_EQ(host, "coucou");
    host = "coucou:42";
    test.parse_port();
    EXPECT_EQ(test._port, 42);
    EXPECT_EQ(host, "coucou");
    host = "coucou:eheh";
    EXPECT_THROW(
        {
            try {
                test.parse_port();
            } catch (HttpError &e) {
                EXPECT_EQ(e.get_code(), BadRequest);
                throw;
            } catch (std::exception &e) {
                throw;
            }
        },
        HttpError
    );
    EXPECT_EQ(host, "coucou");
    host = "coucou:";
    EXPECT_THROW(
        {
            try {
                test.parse_port();
            } catch (HttpError &e) {
                EXPECT_EQ(e.get_code(), BadRequest);
                throw;
            } catch (std::exception &e) {
                throw;
            }
        },
        HttpError
    );
    EXPECT_EQ(host, "coucou");
}
