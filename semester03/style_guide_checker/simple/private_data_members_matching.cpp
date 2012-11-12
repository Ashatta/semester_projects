#include <llvm/Support/Regex.h>
#include "private_data_members_matching.h"

using namespace sgc::private_data_members_matching;

// Visitor
bool Visitor::VisitFieldDecl(clang::FieldDecl *decl)
{
    if (decl->isCXXClassMember()
            && decl->getAccess() == clang::AS_private
            && !match(decl->getNameAsString()))
    {
    }

    return true;
}

bool Visitor::match(std::string const &) const
{
    return false;
}

std::string Visitor::prettyLocation(clang::FieldDecl *decl) const
{
    using namespace clang;

    CXXRecordDecl *classDecl = cast<CXXRecordDecl>(decl->getDeclContext());
    return classDecl->getNameAsString()
            + "::"
            + decl->getNameAsString()
            + " at "
            + decl->getLocation().printToString(*sourceManager());
}

// Consumer
void Consumer::setSourceManager(clang::SourceManager &sm)
{
    visitor_.setSourceManager(sm);
    BaseConsumer::setSourceManager(sm);
}

bool Consumer::HandleTopLevelDecl(clang::DeclGroupRef declGroup)
{
    using namespace clang;

    for (DeclGroupRef::iterator it = declGroup.begin(), end = declGroup.end(); it != end; ++it)
    {
        visitor_.TraverseDecl(*it);
    }
    return true;
}

