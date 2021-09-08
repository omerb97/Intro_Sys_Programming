#include "Exceptions.h"

const char* Exception::what() const noexcept
{
    return "Error: General error";
}
const char* IllegalNodeName::what() const noexcept
{
    return "Error: Illegal node name";
}

const char *IllegalArcNoEndpoint::what() const noexcept
{
    return "Error: Illegal Arc. No endpoint found for arc";
}

const char *IllegalArcStructure::what() const noexcept
{
    return "Error: Illegal Arc. Structure not right";
}

const char *InvalidCommand::what() const noexcept
{
    return "Error: Invalid Command";
}


const char *GraphNotFound::what() const noexcept
{
    return "Error: This graph does not exist";
}

const char *NodeAlreadyExists::what() const noexcept
{
    return "Error: Node already exists";
}

const char *FileNotFound::what() const noexcept
{
    return "Error: File not found";
}

const char *IllegalGraphName::what() const noexcept
{
    return "Error: Illegal graph name";
}

const char *IllegalParentheses::what() const noexcept
{
    return "Error: Unequal parentheses";
}

const char *IllegalBrackets::what() const noexcept
{
    return "Error: Unequal brackets";
}

const char *IllegalNameWithSpace::what() const noexcept
{
    return "Error: Illegal name. Contains a space";
}

const char *IllegalArcSrcDest::what() const noexcept
{
    return "Error: Illegal arc. Same source and destination";
}


