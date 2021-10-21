#include "include/handlers/Base/BaseHandler.h"
#include <Poco/Net/NetException.h>

void BaseHandler::printLogs(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) const {
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "CLIENT HOST: \t" + request.clientAddress().host().toString() << std::endl;
    std::cout << "METHOD: \t" + request.getMethod() << std::endl;
    std::cout << "URI : \t" + request.getURI() << std::endl;
    std::cout << "RESPONSE STATUS: \t" + std::to_string(response.getStatus()) << std::endl;
    std::cout << "CONTENT_TYPE \t" + response.getContentType() << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;
}

void BaseHandler::error404send(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) const {
    std::unique_ptr<ErrorSerializer> serializer(
            new ErrorSerializer(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND));
    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus(serializer->getStatus()));
    Poco::Net::MediaType type("application/json");
    response.setContentType(type);
    response.send() << *serializer->JsonError();

    printLogs(request, response);
}

void BaseHandler::error405send(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) const {
    std::unique_ptr<ErrorSerializer> serializer(
            new ErrorSerializer(Poco::Net::HTTPResponse::HTTPStatus::HTTP_METHOD_NOT_ALLOWED));
    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus(serializer->getStatus()));
    Poco::Net::MediaType type("application/json");
    response.setContentType(type);
    response.send() << *serializer->JsonError();

    printLogs(request, response);
}

void BaseHandler::error401send(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) const {
    std::unique_ptr<ErrorSerializer> serializer(
            new ErrorSerializer(Poco::Net::HTTPResponse::HTTPStatus::HTTP_UNAUTHORIZED));
    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus(serializer->getStatus()));
    Poco::Net::MediaType type("application/json");
    response.setContentType(type);
    response.send() << *serializer->JsonError();

    printLogs(request, response);
}

void BaseHandler::error400send(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response,
                               const std::string &msg) const {
    std::unique_ptr<ErrorSerializer> serializer(
            new ErrorSerializer(Poco::Net::HTTPResponse::HTTPStatus::HTTP_BAD_REQUEST));
    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus(serializer->getStatus()));
    Poco::Net::MediaType type("application/json");
    response.setContentType(type);
    response.send() << *serializer->JsonError(msg);

    printLogs(request, response);
}

void BaseHandler::error500send(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response,
                               const std::string &msg) const {
    std::unique_ptr<ErrorSerializer> serializer(
            new ErrorSerializer(Poco::Net::HTTPResponse::HTTPStatus::HTTP_INTERNAL_SERVER_ERROR));
    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus(serializer->getStatus()));
    Poco::Net::MediaType type("application/json");
    response.setContentType(type);
    response.send() << *serializer->JsonError(msg);

    printLogs(request, response);
}

void BaseHandler::error502send(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response,
                                const std::string &msg) const {
    std::unique_ptr<ErrorSerializer> serializer(
            new ErrorSerializer(Poco::Net::HTTPResponse::HTTPStatus::HTTP_BAD_GATEWAY));
    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus(serializer->getStatus()));
    Poco::Net::MediaType type("application/json");
    response.setContentType(type);
    response.send() << *serializer->JsonError(msg);

    printLogs(request, response);
}

void
BaseHandler::authorizationUser(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) const {
    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS) {
        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
        response.set("Access-Control-Allow-Method", "GET, POST");
        response.set("Access-Control-Allow-Headers", "token, Content-Type, Accept");
        response.send();

        printLogs(request, response);
    } else {
        auto accessToken = request.get("token");
        if (Auth::check_access_token(accessToken) == false)
            throw Poco::Net::NotAuthenticatedException();
    }
}

void BaseHandler::complete204send(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response,
                                  const std::string &msg) const {
    std::unique_ptr<CompleteSerializer> serializer(
            new CompleteSerializer(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NO_CONTENT));
    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus(serializer->getStatus()));
    Poco::Net::MediaType type("application/json");
    response.setContentType(type);
    response.send() << *serializer->JsonComplete(msg);

    printLogs(request, response);
}

FieldName BaseHandler::getNameFieldByQueryParams(const Poco::URI &uri) const {
    for (const auto &x: uri.getQueryParameters())
        if (x.first == "FN") {
            if (x.second == TempQueryKey)
                return FieldName::Temperature;
            else if (x.second == SalinityQueryKey)
                return FieldName::Salinity;
            else if (x.second == SpeedOfSoundQueryKey)
                return FieldName::SpeedOfSound;
            else if (x.second == SeaSurfaceQueryKey)
                return FieldName::SeaSurface;
            else if (x.second == IceQueryKey)
                return FieldName::Ice;
            else if (x.second == EastwardQueryKey)
                return FieldName::EastwardVelocity;
            else if (x.second == NorthwardQueryKey)
                return FieldName::NorthwardVelocity;
            else if (x.second == FlowRateQueryKey)
                return FieldName::FlowRate;
            else
                throw Poco::InvalidArgumentException("Invalid FN");
        }
    throw Poco::InvalidArgumentException("Fn param is not found");
}

TypeSpectr BaseHandler::getNameSpectrByQueryParams(const Poco::URI &uri) const {
    std::unique_ptr<TypeSpectr> result(new TypeSpectr(TypeSpectr::NO));
    for (const auto &x: uri.getQueryParameters())
        if (x.first == "TS") {
            if (x.second == "ALG")
                *result = TypeSpectr::ALG;
            else if (x.second == "ALG2")
                *result = TypeSpectr::ALG2;
            else if (x.second == "FERRET")
                *result = TypeSpectr::FERRET;
            else if (x.second == "GREYSCALE")
                *result = TypeSpectr::GREYSCALE;
            else if (x.second == "NCVIEW")
                *result = TypeSpectr::NCVIEW;
            else if (x.second == "OCCAM")
                *result = TypeSpectr::OCCAM;
            else if (x.second == "OCCAMPASTEL")
                *result = TypeSpectr::OCCAMPASTEL;
            else if (x.second == "RAINBOW")
                *result = TypeSpectr::RAINBOW;
            else if (x.second == "REDBLUE")
                *result = TypeSpectr::REDBLUE;
            else if (x.second == "ICE")
                *result = TypeSpectr::ICE_SPECTR;
            else if (x.second == "SST")
                *result = TypeSpectr::SST;
        }
    if (*result == TypeSpectr::NO)
        throw Poco::InvalidArgumentException("Неверный тип спектра");
    return *result;
}

ZoneName BaseHandler::getNameZoneByQueryParams(const Poco::URI &uri) const {

    if (uri.toString().substr(5, uri.toString().size()).substr(0, 5) == "world") return ZoneName::WORLD;
    if (uri.toString().substr(5, uri.toString().size()).substr(0, 6) == "arctic") return ZoneName::ARCTIC;
    if (uri.toString().substr(5, uri.toString().size()).substr(0, 7) == "Nesting") return ZoneName::NESTING;
    if (uri.toString().substr(5, uri.toString().size()).substr(0, 7) == "Dynamic") return ZoneName::DYNAMIC;

    if (uri.toString().substr(5, uri.toString().size()).substr(0, 8) == "Climatic") {
        if (uri.toString().substr(5, uri.toString().size()).substr(0, 10) == "ClimaticSD") return ZoneName::CLIMATIC_SD;
        return ZoneName::CLIMATIC;
    }

    throw Poco::InvalidArgumentException("ZoneName is not Correct");
}

const size_t BaseHandler::getTimeIndexByQueryParams(const Poco::URI &uri) const {

    int time = -1;

    for (const auto &x: uri.getQueryParameters()) {
        if (x.first == "time")
            time = std::atoi(x.second.c_str());
    }
    if (time == -1)
        throw Poco::InvalidArgumentException("Time Value Not Found");
    if (time >= 4)
        throw Poco::InvalidArgumentException("Time value don't be more or equal 4 [ 0, 1 ,2 ,3 ]");
    if (time < 0)
        throw Poco::InvalidArgumentException("Time values don't be less 0 [ 0, 1 ,2 ,3 ]");

    return size_t(time);
}

const std::string BaseHandler::getTimeForFileByQueryParams(const Poco::URI &uri) const {
    for (const auto &x: uri.getQueryParameters()) {
        if (x.first == "time_file")
            return "-" + x.second;
    }

    throw Poco::InvalidArgumentException("time_file Value Not Found");
}

const std::string BaseHandler::getDateByQueryParams(const Poco::URI &uri) const {
    for (const auto &x: uri.getQueryParameters()) {
        if (x.first == "date")
            return "/" + x.second;
    }

    throw Poco::InvalidArgumentException("Date Value Not Found");
}

const std::pair<std::string, std::string> BaseHandler::getDuoDateByQueryParams(const Poco::URI &uri) const {
    std::pair<std::string, std::string> result;
    for (const auto &x: uri.getQueryParameters()) {
        if (x.first == "date1") {
            result.first = x.second;
        }
        if (x.first == "date2") {
            result.second = x.second;
        }
    }
    if (result.first.empty())
        throw Poco::InvalidArgumentException("Date1 Value Not Found");
    if (result.second.empty())
        throw Poco::InvalidArgumentException("Date2 Value Not Found");

    return result;
}

const size_t BaseHandler::getDepthIndexByQueryParams(const Poco::URI &uri) const {

    int k = -1;
    for (const auto &x: uri.getQueryParameters()) {
        if (x.first == "k")
            k = std::atoi(x.second.c_str());
    }
    if (k == -1)
        throw Poco::InvalidArgumentException("Not Param K");
    if (k < 0)
        throw Poco::InvalidArgumentException("Not valid param K too Small");
    if (k >= 49)
        throw Poco::InvalidArgumentException("Not valid param K Too Many");

    return size_t(k);
}

const float BaseHandler::getLat(const Poco::URI &uri) const {
    for (const auto &x: uri.getQueryParameters()) {
        if (x.first == "lat")
            return std::atof(x.second.c_str());
    }

    throw Poco::InvalidArgumentException("Parametr Lat is None");
}

const float BaseHandler::getLon(const Poco::URI &uri) const {
    for (const auto &x: uri.getQueryParameters()) {
        if (x.first == "lon")
            return std::atof(x.second.c_str());
    }

    throw Poco::InvalidArgumentException("Parametr Lon is None");
}

const std::pair<float, float> BaseHandler::getStartLatLon(const Poco::URI &uri) const {
    std::unique_ptr<std::pair<float, float>> start(new std::pair<float, float>(-999.0, -999.0));
    for (const auto &x: uri.getQueryParameters()) {
        if (x.first == "lat_s")
            start->first = std::atof(x.second.c_str());
        if (x.first == "lon_s")
            start->second = std::atof(x.second.c_str());
    }
    if (start->first == -999.0)
        throw Poco::InvalidArgumentException("Lat Left Top is None");
    if (start->second == -999.0)
        throw Poco::InvalidArgumentException("Lon Left Top is None");
    return *start;
}

const std::pair<float, float> BaseHandler::getEndLatLon(const Poco::URI &uri) const {
    std::unique_ptr<std::pair<float, float>> end(new std::pair<float, float>);
    for (const auto &x: uri.getQueryParameters()) {
        if (x.first == "lat_e")
            end->first = std::atof(x.second.c_str());
        if (x.first == "lon_e")
            end->second = std::atof(x.second.c_str());
    }
    if (end->first == -999.0)
        throw Poco::InvalidArgumentException("Lat Bottom Rigth is None");
    if (end->second == -999.0)
        throw Poco::InvalidArgumentException("Lon Bottom Rigth is None");
    return *end;
}

const bool BaseHandler::getReverseParam(const Poco::URI &uri) const {
    for (const auto &x: uri.getQueryParameters()) {
        if (x.first == "reverse")
            return x.second == "true" ? true : false;
    }

    throw Poco::InvalidArgumentException("Parametr Reverse is None");
}

void BaseHandler::isDigitParam(ParamType &inParamValue, const std::string &outParamValue) const {
    if (!outParamValue.empty()) {
        for (const auto &y: outParamValue) {
            if (((y < '0' || y > '9') && y != '-' && y != '.'))
                throw Poco::InvalidArgumentException();
        }
        if (std::is_same<ParamType, float>::value)
            inParamValue = std::atof(outParamValue.c_str());
        if (std::is_same<ParamType, int>::value)
            inParamValue = std::atoi(outParamValue.c_str());
    }
}
