/* #############################################################################

               """          Message_test.hpp
        -\-    _|__
         |\___/  . \        Created on 13 May. 2024 at 13:54
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_MESSAGE_TEST_HPP_
#define INCLUDE_SRC_MESSAGE_TEST_HPP_

#include "HttpError.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include <map>
#include <string>

typedef struct s_test_target {
    std::string name;
    std::string c1;
    std::string c2;
    std::string c3;
} t_test_target;

typedef struct s_test_messages {
    std::string                         name;
    std::string                         request;
    std::map<std::string, std::string>  headers;
    std::string                         target;
    HttpMethod                          method;
    bool                                absolute_form;
    HttpCode                            error;
} t_test_messages;

static const t_test_target MessageTargetSuiteValues[] {
    {
        "normal", "/dev HTTP/1.1", "/dev", ""
    }, {
        "no_slash", "dev HTTP/1.1", "BadRequest", ""
    }, {
        "empty", " HTTP/1.1", "BadRequest", ""
    }, {
        "empty_2", "", "BadRequest", ""
    }, {
        "spaces", "/dev?using spaces is really\twrong HTTP/1.1", "MovedPermanently",
        "/dev?using%20spaces%20is%20really%09wrong"
    }, {
        "URI_too_long",
        "/super/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo" "l/cool/cool HTTP/1.1",
        "URITooLong", ""
    }, {
        "no_protocol", "/dev?using AHAH/1.0", "BadRequest", ""
    }, {
        "no_more_spaces", "/dev?usingAHAH/1.0", "BadRequest", ""
    }, {
        "absolute_form", "http://host.com/dev?using HTTP/1.1", "/dev?using", "host.com"
    }, {
        "absolute_broken", "http:///dev?using HTTP/1.1", "BadRequest", ""
    }, {
        "https_absolute_form", "https://host.com/dev?using HTTP/1.1", "BadRequest", ""
    }, {
        "spaces_absolute_form", "http://host.com/dev?wrong using HTTP/1.1", "MovedPermanently",
        "http://host.com/dev?wrong%20using"
    }, {
        "root", "/ HTTP/1.1", "/", ""
    },
};

static const t_test_target MessageParseHeaderLineSuiteValues[] {
    {
        "basic", "Host: test", "Host", "test"
    }, {
        "nospaces", "Host:test", "Host", "test"
    }, {
        "spaces", "Host: \t test\t\t      ", "Host", "test"
    }, {
        "sep_in_name", "Host :", "Host", "BadRequest"
    }, {
        "no_value", "Host:", "Host", "BadRequest"
    }, {
        "no_value2", "Host: ", "Host", "BadRequest"
    }, {
        "no_value3", "Host:             ", "Host", "BadRequest"
    }, {
        "sp_in_val", "coucou: je suis heureux  ", "coucou", "je suis heureux"
    }, {
        "no_name", ":eheheh", "", "BadRequest"
    }, {
        "no_colon", "eheheh", "eheheh", "BadRequest"
    }, {
        "nothing", "", "", "BadRequest"
    },
};

static const t_test_messages MessageTestData[] {
    // {
    //     "name",
    //     "request",
    //     {
    //         {"Host", "coucou"},
    //     },
    //     "target",
    //     "method",
    //     false,
    //     unset
    // },
    {
        "base",
        "GET / HTTP/1.1\r\n"
        "Host: www.example.com\r\n"
        "Infos: il_n'y_a_rien_ici\r\n"
        "coucou: jesuisheureux\r\n"
        "\r\n"
        "Eheheheheh voici des donnees\r\n",
        {
            {
                "Host", "www.example.com"
            }, {
                "Infos", "il_n'y_a_rien_ici"
            }, {
                "coucou", "jesuisheureux"
            },
        }, "/", GET, false, unset
    },
    {
        "no_headers",
        "GET / HTTP/1.1\r\n"
        "\r\n"
        "Il n'y a pas de headers !!\r\n",
        {
        }, "/", GET, false, BadRequest
    },
    {
        "no_headers_absolute",
        "GET http://superjesuisheureux.com/ HTTP/1.1\r\n"
        "\r\n"
        "Il n'y a pas de headers !!\r\n",
        {
            {"Host", "superjesuisheureux.com"}
        }, "/", GET, true, unset
    },
};

#endif  // INCLUDE_SRC_MESSAGE_TEST_HPP_
