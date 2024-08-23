/* #############################################################################

               """          ClientRequest_test.hpp
        -\-    _|__
         |\___/  . \        Created on 13 May. 2024 at 13:54
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_MESSAGE_TEST_HPP_
#define INCLUDE_SRC_MESSAGE_TEST_HPP_

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
        std::string                          name;
        std::string                          request;
        std::map< std::string, std::string > headers;
        std::string                          target;
        HttpMethod                           method;
        bool                                 absolute_form;
        HttpCode                             error;
} t_test_messages;

static const t_test_target ClientRequestTargetSuiteValues[]{
    {
        "normal",
        "/dev HTTP/1.1\r\n",
        "/dev",
        "",
    },
    {
        "no_slash",
        "dev HTTP/1.1\r\n",
        "BadRequest",
        "",
    },
    {
        "empty",
        " HTTP/1.1\r\n",
        "BadRequest",
        "",
    },
    {
        "empty_2",
        "",
        "BadRequest",
        "",
    },
    {
        "spaces",
        "/dev?using spaces is really\twrong HTTP/1.1\r\n",
        "MovedPermanently",
        "/dev?using%20spaces%20is%20really%09wrong",
    },
    {
        "URI_too_long",
        "/super/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/coo"
        "l/cool/cool/cool/cool/cool/cool/cocool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/c"
        "ool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool/cool HTTP/1.1\r\n",
        "URITooLong",
        "",
    },
    {
        "no_protocol",
        "/dev?using AHAH/1.0\r\n",
        "BadRequest",
        "",
    },
    {
        "no_more_spaces",
        "/dev?usingAHAH/1.0\r\n",
        "BadRequest",
        "",
    },
    {
        "absolute_form",
        "http://host.com/dev?using HTTP/1.1\r\n",
        "/dev?using",
        "host.com",
    },
    {
        "absolute_broken",
        "http:///dev?using HTTP/1.1\r\n",
        "BadRequest",
        "",
    },
    {
        "https_absolute_form",
        "https://host.com/dev?using HTTP/1.1\r\n",
        "BadRequest",
        "",
    },
    {
        "spaces_absolute_form",
        "http://host.com/dev?wrong using HTTP/1.1\r\n",
        "MovedPermanently",
        "http://host.com/dev?wrong%20using",
    },
    {
        "root",
        "/ HTTP/1.1\r\n",
        "/",
        "",
    },
};

static const t_test_target ClientRequestParseHeaderLineSuiteValues[]{
    {
        "basic",
        "Host: test",
        "Host",
        "test",
    },
    {
        "nospaces",
        "Host:test",
        "Host",
        "test",
    },
    {
        "spaces",
        "Host: \t test\t\t      ",
        "Host",
        "test",
    },
    {
        "sep_in_name",
        "Host :",
        "Host",
        "BadRequest",
    },
    {
        "sep_in_name2",
        "   Host:",
        "Host",
        "BadRequest",
    },
    {
        "sep_in_name3",
        "  Host :",
        "Host",
        "BadRequest",
    },
    {
        "no_value",
        "Host:",
        "Host",
        "BadRequest",
    },
    {
        "no_value2",
        "Host: ",
        "Host",
        "BadRequest",
    },
    {
        "no_value3",
        "Host:             ",
        "Host",
        "BadRequest",
    },
    {
        "sp_in_val",
        "coucou: je suis heureux  ",
        "coucou",
        "je suis heureux",
    },
    {
        "no_name",
        ":eheheh",
        "",
        "BadRequest",
    },
    {
        "no_colon",
        "eheheh",
        "eheheh",
        "BadRequest",
    },
    {
        "nothing",
        "",
        "",
        "BadRequest",
    },
};

static const t_test_messages ClientRequestTestData[]{
    // {
    // "name",
    // "request",
    // {
    // {"Host", "coucou"},
    // },
    // "target",
    // "method",
    // false,
    // unset
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
            { "Host", "www.example.com" },
            { "Infos", "il_n'y_a_rien_ici" },
            { "coucou", "jesuisheureux" },
        },
        "/",
        GET,
        false,
        unset,
    },
    {
        "no_body",
        "GET / HTTP/1.1\r\n"
        "Host: www.example.com\r\n"
        "Infos: il_n'y_a_rien_ici\r\n"
        "coucou: jesuisheureux\r\n"
        "\r\n", // no
                // data
        {
            { "Host", "www.example.com" },
            { "Infos", "il_n'y_a_rien_ici" },
            { "coucou", "jesuisheureux" },
        },
        "/",
        GET,
        false,
        unset,
    },
    {
        "no_headers",
        "GET / HTTP/1.1\r\n"
        "\r\n"
        "Il n'y a pas de headers !!\r\n",
        {},
        "/",
        GET,
        false,
        BadRequest,
    },
    {
        "no_headers_absolute",
        "GET http://superjesuisheureux.com/ HTTP/1.1\r\n"
        "\r\n"
        "Il n'y a pas de headers !!\r\n",
        { { "Host", "superjesuisheureux.com" } },
        "/",
        GET,
        true,
        unset,
    },
};

#endif // INCLUDE_SRC_MESSAGE_TEST_HPP_
