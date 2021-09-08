
#ifndef FINAL_PROJECT_EXCEPTIONS_H
#define FINAL_PROJECT_EXCEPTIONS_H
#include <string>


class ArcException : public std::exception {};


class Exception : public std::exception
{
public:
    const char* what() const noexcept override;
};
class IllegalNodeName : public std::exception
{
public:
    const char* what() const noexcept override;
};

class IllegalArcNoEndpoint : public ArcException
{
public:
    const char* what() const noexcept override;
};

class IllegalArcStructure : public ArcException
{
public:
    const char* what() const noexcept override;
};


class InvalidCommand : public std::exception
{
public:
    const char* what() const noexcept override;
};

class GraphNotFound : public std::exception
{
public:
    const char* what() const noexcept override;
};

class NodeAlreadyExists : public std::exception
{
public:
    const char* what() const noexcept override;
};
class FileNotFound : public std::exception
{
public:
    const char* what() const noexcept override;
};
class IllegalGraphName : public std::exception
{
public:
    const char* what() const noexcept override;
};

class IllegalParentheses : public std::exception
{
public:
    const char* what() const noexcept override;
};

class IllegalBrackets : public std::exception
{
public:
    const char* what() const noexcept override;
};

class IllegalNameWithSpace : public std::exception
{
public:
    const char* what() const noexcept override;
};

class IllegalArcSrcDest : public ArcException
{
public:
    const char *what() const noexcept override;

};

#endif
