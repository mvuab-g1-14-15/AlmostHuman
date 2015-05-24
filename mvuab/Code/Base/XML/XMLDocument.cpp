#include "XMLDocument.h"
#include "Logger\Logger.h"
#include "Utils\Defines.h"

//-----------------------------------------------------------------------------------------------------------------------------------------
CXMLDocument::CXMLDocument( const std::string& aFile )
    : mFile( aFile )
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------
CXMLDocument::~CXMLDocument()
{
}

bool CXMLDocument::Load()
{
    bool lOk(true);
    pugi::xml_parse_result lResult = mDocument.load_file(mFile.c_str());

    if( lResult )
    {
        pugi::xml_parse_status lDocumentStatus = lResult.status;
        if( lDocumentStatus != pugi::status_ok )
        {
            lOk = false;
            switch( lDocumentStatus )
            {
                case pugi::status_file_not_found:
                    LOG_ERROR_APPLICATION("File %s not fount", mFile.c_str() );
                    break;
                case pugi::status_io_error:
                    LOG_ERROR_APPLICATION("Error reading from file/stream");
                    break;
                case pugi::status_out_of_memory:
                    LOG_ERROR_APPLICATION("Could not allocate memory");
                    break;
                case pugi::status_internal_error:
                    LOG_ERROR_APPLICATION("Internal error occurred");
                    break;
                case pugi::status_unrecognized_tag:
                    LOG_ERROR_APPLICATION("Parser could not determine tag type");
                    break;
                case pugi::status_bad_comment:
                    LOG_ERROR_APPLICATION("Parsing error occurred while parsing comment" );
                    break;
                case pugi::status_bad_cdata:
                    LOG_ERROR_APPLICATION("Parsing error occurred while parsing CDATA section");
                    break;
                case pugi::status_bad_doctype:
                    LOG_ERROR_APPLICATION("Parsing error occurred while parsing document type declaration" );
                    break;
                case pugi::status_bad_pcdata:
                    LOG_ERROR_APPLICATION("Parsing error occurred while parsing PCDATA section" );
                    break;
                case pugi::status_bad_start_element:
                    LOG_ERROR_APPLICATION("Parsing error occurred while parsing start element tag" );
                    break;
                case pugi::status_bad_attribute:
                    LOG_ERROR_APPLICATION("Parsing error occurred while parsing element attribute" );
                    break;
                case pugi::status_bad_end_element:
                    LOG_ERROR_APPLICATION("Parsing error occurred while parsing end element tag" );
                    break;
                case pugi::status_end_element_mismatch:
                    LOG_ERROR_APPLICATION("There was a mismatch of start-end tags (closing tag had incorrect name, some tag was not closed or there was an excessive closing tag)" );
                    break;
                case pugi::status_no_document_element:
                    LOG_ERROR_APPLICATION("Parsing resulted in a document without element nodes");
                    break;
                case pugi::status_append_invalid_root:
                    LOG_ERROR_APPLICATION("Unable to append nodes since root type is not node_element or node_document (exclusive to xml_node::append_buffer)");
                    break;
            }
        }
    }
    else
    {
        lOk = false;
        LOG_ERROR_APPLICATION("%s", lResult.description() );
    }

    return lOk;
}

CXMLTreeNode CXMLDocument::GetNode( const std::string aNode ) const
{
    return CXMLTreeNode(mDocument.child(aNode.c_str()));
}