#ifndef INCLUDE_SRC_SERVERCONFSETTER_HPP_
#define INCLUDE_SRC_SERVERCONFSETTER_HPP_

#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "ServerConfFields.hpp"
#include <set>
#include <string>
#include <vector>

std::vector< std::string > setFieldServerName(const ValueList &values);
int                        setFieldListen(const ValueList &values);
std::string                setFieldRoot(const ValueList &values);
std::vector< std::string > setFieldIndex(const ValueList &values);
bool                       setFieldAutoindex(const ValueList &values);
std::set< HttpMethod >     setFieldMethods(const ValueList &values);
int                        setFieldMaxBodySize(const ValueList &values);
HttpCode                   setFieldErrorPageCode(const ValueList &values);
std::string                setFieldLocationPath(const std::string &values);
std::string                setFieldUploadPath(const ValueList &values);
HttpCode                   setFieldRewriteCode(const ValueList &values);
std::string                setFieldCgiPath(const ValueList &values);
std::string                setFieldFileExt(const ValueList &values);

#endif // INCLUDE_SRC_SERVERCONFSETTER_HPP_
