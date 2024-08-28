/* #####################################################################################################################

               """          FakeRoute.hpp
        -\-    _|__
         |\___/  . \        Created on 28 Jul. 2024 at 17:12
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#ifndef INCLUDE_TEST_FAKEROUTE_HPP_
#define INCLUDE_TEST_FAKEROUTE_HPP_

#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "gtest/gtest_prod.h"
#include <set>
#include <string>
#include <vector>

class ResponseBuildStateFixture;
class LocationTestFixture;

class FakeRoute {
    public:
        FakeRoute() {};

        FakeRoute(
            HttpCode                          _getRedirCode_,
            bool                              _hasAutoindex_,
            bool                              _hasCgiExtension_,
            bool                              _hasCgiPath_,
            bool                              _hasRedir_,
            bool                              _hasUpload_,
            const std::string                &_getCgiExtension_,
            const std::string                &_getCgiPath_,
            const std::string                &_getLocation_,
            const std::string                &_getRedirPage_,
            const std::string                &_getRootDir_,
            const std::string                &_getUploadPath_,
            // const std::string              &_getRoutePath_,
            // const std::string              &_getUploadPath_,
            const std::set< HttpMethod >     &_getMethods_,
            const std::vector< std::string > &_getIndexPage_
        ) :
            _getRedirCode(_getRedirCode_),
            _hasAutoindex(_hasAutoindex_),
            _hasCgiExtension(_hasCgiExtension_),
            _hasCgiPath(_hasCgiPath_),
            _hasRedir(_hasRedir_),
            _hasUpload(_hasUpload_),
            _getCgiExtension(_getCgiExtension_),
            _getCgiPath(_getCgiPath_),
            _getLocation(_getLocation_),
            _getRedirPage(_getRedirPage_),
            _getRootDir(_getRootDir_),
            _getUploadPath(_getUploadPath_),
            // _getRoutePath(_getRoutePath_),
            _getMethods(_getMethods_),
            _getIndexPage(_getIndexPage_) {
            if (_getUploadPath == "")
                _getUploadPath = _getRootDir;
            // if (workdir != "") {
            //     if (*workdir.rbegin() == '/')
            //         workdir.resize(workdir.length() - 1);
            //     _getUploadPath = workdir + (*_getUploadPath.begin() == '/' ? "" : "/") + _getUploadPath;
            //     _getRootDir    = workdir + (*_getRootDir.begin() == '/' ? "" : "/") + _getRootDir;
            // }
        }

        HttpCode getRedirCode() const { return _getRedirCode; }

        bool hasAutoindexSet() const { return _hasAutoindex; }

        bool hasCgiExtensionSet() const { return _hasCgiExtension; }

        bool hasCgiPathSet() const { return _hasCgiPath; }

        bool hasRedirSet() const { return _hasRedir; }

        bool hasUploadSet() const { return _hasUpload; }

        const std::string &getCgiExtension() const { return _getCgiExtension; }

        const std::string &getCgiPath() const { return _getCgiPath; }

        const std::string &getLocation() const { return _getLocation; }

        const std::string &getRedirPage() const { return _getRedirPage; }

        const std::string &getRootDir() const { return _getRootDir; }

        const std::string &getUploadPath() const { return _getUploadPath; }

        // const std::string &getRoutePath() const { return _getRoutePath; }

        const std::set< HttpMethod > &getMethods() const { return _getMethods; }

        const std::vector< std::string > &getIndexPage() const { return _getIndexPage; }

        static std::string workdir; // used to set workdir to debug and or test

    private:
        HttpCode                   _getRedirCode;
        bool                       _hasAutoindex;
        bool                       _hasCgiExtension;
        bool                       _hasCgiPath;
        bool                       _hasRedir;
        bool                       _hasUpload;
        std::string                _getCgiExtension;
        std::string                _getCgiPath;
        std::string                _getLocation;
        std::string                _getRedirPage;
        std::string                _getRootDir;
        std::string                _getUploadPath;
        // std::string              _getRoutePath;
        std::set< HttpMethod >     _getMethods;
        std::vector< std::string > _getIndexPage;

        friend ResponseBuildStateFixture;
        FRIEND_TEST(LocationTestSuite, BuildPathTest);
        FRIEND_TEST(LocationTestSuite, BuildPathRedirectTest);
};

#endif // INCLUDE_TEST_FAKEROUTE_HPP_
