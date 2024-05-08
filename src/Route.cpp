#include "Route.hpp"
#include "HttpMethods.hpp"

Route::Route() :
	_autoindex(false),
	_methods(),
	_rootDir("www/html"),
	_indexPage(1, "index.html"),
	_uploadPath(""),
	_redirCode(""),
	_redirPage(""),
	_cgiPath(""),
	_cgiExtension("")
{
}

Route::~Route()
{
}

bool Route::getAutoindex() const
{
	return _autoindex;
}

std::vector<HttpMethod> Route::getMethods() const
{
	return _methods;
}

std::vector<std::string> Route::getIndexPage() const
{
	return _indexPage;
}

std::string Route::getUploadPath() const
{
	return _uploadPath;
}

std::string Route::getRedirCode() const
{
	return _redirCode;
}

std::string Route::getRedirPage() const
{
	return _redirPage;
}

std::string Route::getCgiPath() const
{
	return _cgiPath;
}

std::string Route::getCgiExtension() const
{
	return _cgiExtension;
}

bool Route::hasAutoindex() const
{
	return _autoindexSet;
}

bool Route::hasMethods() const
{
	return _methodsSet;
}

bool Route::hasRoot() const
{
	return _rootDirSet;
}

bool Route::hasIndexPage() const
{
	return _indexPageSet;
}

bool Route::hasRedir() const
{
	return _redirectionSet;
}

bool Route::hasUpload() const
{
	return _uploadSet;
}

bool Route::hasCgi() const
{
	return _cgiSet;
}
