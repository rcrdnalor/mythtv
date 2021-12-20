/////////////////////////////////////////////////////////////////////////////
// Program Name: xsd.cpp
// Created     : Feb. 20, 2012
//
// Purpose     : XSD Generation Class
//
// Copyright (c) 2012 David Blain <dblain@mythtv.org>
//
// Licensed under the GPL v2 or later, see COPYING for details
//
/////////////////////////////////////////////////////////////////////////////

#include <QCoreApplication>

#include "mythlogging.h"
#include "http/mythxsd.h"
#include "http/mythmimedatabase.h"
#include "http/mythhttpmetaservice.h"
#include "http/mythhttprequest.h"
#include "http/mythhttpresponse.h"


/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

// The code for  creating the enum in xsd does not work
// Since there is only one enum n the entire system, treat it as a string

// HTTPResponse MythXSD::GetEnumXSD( HTTPRequest2 pRequest, const QString& sEnumName )
// {
//     if (sEnumName.isEmpty())
//         return Error(pRequest, QString( "XSD request on empty invalid enum name"));

//     // ----------------------------------------------------------------------
//     // sEnumName needs to be in class.enum format
//     // ----------------------------------------------------------------------

//     if (sEnumName.count('.') != 1 )
//         return Error(pRequest, QString( "XSD request invalid enum name %1").arg(sEnumName));

//     QStringList lstTypeParts = sEnumName.split( '.' );

//     // ----------------------------------------------------------------------
//     // Create Parent object so we can get to its metaObject
//     // ----------------------------------------------------------------------

//     QString sParentFQN = lstTypeParts[0];
// #if QT_VERSION < QT_VERSION_CHECK(6,0,0)
//     int nParentId = QMetaType::type( sParentFQN.toUtf8() );

//     // ----------------------------------------------------------------------
//     // Check for things that were formerly registered as both 'Foo' and 'Foo*'
//     // ----------------------------------------------------------------------
//     if (nParentId == QMetaType::UnknownType)
//     {
//         QString sFQN = sParentFQN + "*";
//         nParentId = QMetaType::type( sFQN.toUtf8() );
//     }

//     // ----------------------------------------------------------------------
//     // if a DataContract type, we need to prefix name with DTC::
//     // These types are all pointers to objects, so we also need to add "*"
//     // ----------------------------------------------------------------------

//     if (nParentId == QMetaType::UnknownType)
//     {
//         QString sFQN = "V2" + sParentFQN + "*";
//         nParentId = QMetaType::type( sFQN.toUtf8() );
//     }

//     if (nParentId == QMetaType::UnknownType)
//         return Error(pRequest, QString( "XSD request unknown enum name %1").arg(sEnumName));
//     const QMetaObject *pMetaObject = QMetaType::metaObjectForType(nParentId);
// #else
//     QMetaType metaType = QMetaType::fromName( sParentFQN.toUtf8() );
//     if (metaType.id() == QMetaType::UnknownType)
//         metaType = QMetaType::fromName( sParentFQN.toUtf8() + "*" );
//     if (metaType.id() == QMetaType::UnknownType)
//         metaType = QMetaType::fromName( "V2" + sParentFQN.toUtf8() + "*" );
//     if (metaType.id() == QMetaType::UnknownType)
//         return Error(pRequest, QString( "XSD request unknown enum name %1").arg(sEnumName));
//     const QMetaObject *pMetaObject = metaType.metaObject();
// #endif

//     if (pMetaObject == nullptr)
//         return Error(pRequest, QString( "XSD cannot find enum name %1").arg(sEnumName));

//     // ----------------------------------------------------------------------
//     // Now look up enum
//     // ----------------------------------------------------------------------

//     int nEnumIdx = pMetaObject->indexOfEnumerator( lstTypeParts[1].toUtf8() );

//     if (nEnumIdx < 0 )
//         return Error(pRequest, QString( "XSD cannot find values for enum name %1").arg(sEnumName));

//     QMetaEnum metaEnum = pMetaObject->enumerator( nEnumIdx );

//     // ----------------------------------------------------------------------
//     // render xsd for this enum
//     //
//     //    <xs:simpleType name="RecordingInfo.RecordingDupMethodEnum">
//     //        <xs:restriction base="xs:string">
//     //            <xs:enumeration value="kDupCheckNone">
//     //                <xs:annotation>
//     //                    <xs:appinfo>
//     //                        <EnumerationValue xmlns="http://schemas.microsoft.com/2003/10/Serialization/">1</EnumerationValue>
//     //                    </xs:appinfo>
//     //                </xs:annotation>
//     //            </xs:enumeration>
//     //            <xs:enumeration value="kDupCheckSub">
//     //                <xs:annotation>
//     //                    <xs:appinfo>
//     //                        <EnumerationValue xmlns="http://schemas.microsoft.com/2003/10/Serialization/">2</EnumerationValue>
//     //                    </xs:appinfo>
//     //                </xs:annotation>
//     //            </xs:enumeration>
//     //        </xs:restriction>
//     //    </xs:simpleType>
//     //
//     //    <xs:element name="RecordingInfo.RecordingDupMethodEnum" type="tns:RecordingInfo.RecordingDupMethodEnum" nillable="true"/>
//     // ----------------------------------------------------------------------

//     if (!pRequest->m_queries.contains( "raw" ))
//     {
//         appendChild( createProcessingInstruction( "xml-stylesheet",
//                         R"(type="text/xsl" href="/xslt/enum.xslt")" ));
//     }

//     // ----------------------------------------------------------------------
//     // Create xs:simpleType structure
//     // ----------------------------------------------------------------------

//     QDomElement oTypeNode     = createElement( "xs:simpleType"  );
//     QDomElement oRestrictNode = createElement( "xs:restriction" );

//     oTypeNode    .setAttribute( "name", sEnumName   );
//     oRestrictNode.setAttribute( "base", "xs:string" );

//     oTypeNode.appendChild( oRestrictNode  );

//     for( int nIdx = 0; nIdx < metaEnum.keyCount(); nIdx++)
//     {
//         QDomElement oEnum = createElement( "xs:enumeration" );

//         oEnum.setAttribute( "value", metaEnum.key( nIdx ));

//         // ------------------------------------------------------------------
//         // Add appInfo to store numerical value & translated text
//         // ------------------------------------------------------------------

//         QDomElement oAnn      = createElement( "xs:annotation"    );
//         QDomElement oApp      = createElement( "xs:appinfo"       );
//         QDomElement oEnumVal  = createElement( "EnumerationValue" );
//         QDomElement oEnumDesc = createElement( "EnumerationDesc"  );

//         // The following namespace is needed for visual studio to generate negative enums correctly.
//         oEnumVal.setAttribute("xmlns", "http://schemas.microsoft.com/2003/10/Serialization/");

//         oEnum.appendChild( oAnn      );
//         oAnn .appendChild( oApp      );
//         oApp .appendChild( oEnumVal  );
//         oApp .appendChild( oEnumDesc );

//         QString sFQNKey = sEnumName + "." + metaEnum.key( nIdx );

//         oEnumVal .appendChild( createTextNode( QString::number( metaEnum.value( nIdx ))));
//         oEnumDesc.appendChild( createTextNode( QCoreApplication::translate("Enums",
//                                                                            sFQNKey.toUtf8() )));

//         oRestrictNode.appendChild( oEnum );
//     }

//     // ----------------------------------------------------------------------

//     QDomElement oElementNode = createElement( "xs:element" );

//     oElementNode.setAttribute( "name"    , sEnumName );
//     oElementNode.setAttribute( "type"    , "tns:" + sEnumName );
//     oElementNode.setAttribute( "nillable", "true" );

//     // ----------------------------------------------------------------------
//     //
//     // ----------------------------------------------------------------------

//     QDomElement oRoot = CreateSchemaRoot();

//     oRoot.appendChild( oTypeNode    );
//     oRoot.appendChild( oElementNode );

//     appendChild( oRoot );

//     // ----------------------------------------------------------------------
//     // Return xsd doc to caller
//     // ----------------------------------------------------------------------

//     // Create the XML result
//     auto data = MythHTTPData::Create(toByteArray());
//     data->m_mimeType = MythMimeDatabase().MimeTypeForName("application/xml");
//     data->m_cacheType = HTTPETag | HTTPShortLife;
//     return MythHTTPResponse::DataResponse(pRequest, data);
// }

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

HTTPResponse MythXSD::GetXSD( HTTPRequest2 pRequest, QString sTypeName )
{
    bool    bIsArray     = false;
    bool    bIsMap       = false;

    if (sTypeName.isEmpty())
        return Error(pRequest, QString( "XSD request on empty name" ));


    // ----------------------------------------------------------------------
    // Is this a special type name?
    // ----------------------------------------------------------------------

    if (sTypeName.startsWith( "ArrayOf" ))
    {
        bIsArray = true;
        sTypeName = sTypeName.mid( 7 );
    }

    if (sTypeName.startsWith( "MapOfString" ))
    {
        bIsMap       = true;
        sTypeName    = sTypeName.mid( 11 );
    }

    // ----------------------------------------------------------------------
    // Check to see if one of the Qt Types we need to handle special
    // ----------------------------------------------------------------------

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    int id = QMetaType::type( sTypeName.toUtf8() );

    // ----------------------------------------------------------------------
    // Check for things that were formerly registered as both 'Foo' and 'Foo*'
    // ----------------------------------------------------------------------
    if (id == QMetaType::UnknownType)
    {
        QString sFQN = sTypeName + "*";
        id = QMetaType::type( sFQN.toUtf8() );
    }

    // ----------------------------------------------------------------------
    // if a DataContract type, we need to prefix name with DTC::
    // These types are all pointers to objects, so we also need to add "*"
    // ----------------------------------------------------------------------

    if (id == QMetaType::UnknownType)
    {
        QString sFQN = "V2" + sTypeName;
        id = QMetaType::type( sFQN.toUtf8() );
    }
    if (id == QMetaType::UnknownType)
    {
        QString sFQN = "V2" + sTypeName + "*";
        id = QMetaType::type( sFQN.toUtf8() );
    }
    // if (id == QMetaType::UnknownType)
    //     return Error(pRequest, QString( "XSD request on unknown type %1").arg(sTypeName));
#else
    QMetaType metaType = QMetaType::fromName( sTypeName.toUtf8() );
    if (metaType.id() == QMetaType::UnknownType)
        metaType = QMetaType::fromName( sTypeName.toUtf8() + "*" );
    if (metaType.id() == QMetaType::UnknownType)
        metaType = QMetaType::fromName( "V2" + sTypeName.toUtf8() + "*" );
    // if (metaType.id() == QMetaType::UnknownType)
    //     return Error(pRequest,QString( "XSD request on unknown type %1").arg(sTypeName));

    int id = metaType.id();
#endif

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------

    if (!(bIsArray || bIsMap) && (id < QMetaType::User))
        return Error(pRequest,QString( "XSD request on unsupported type %1").arg(sTypeName));

    // ------------------------------------------------------------------
    // Need to create an instance of the class to access it's metadata.
    // ------------------------------------------------------------------

    // --------------------------------------------------------------
    // Render XSD
    // --------------------------------------------------------------

    if (!pRequest->m_queries.contains( "raw" ))
    {
        appendChild( createProcessingInstruction( "xml-stylesheet",
                        R"(type="text/xsl" href="/xslt/class.xslt")" ));
    }

    if (bIsArray)
    {
        RenderArrayXSD( pRequest, sTypeName, id >= QMetaType::User );
    }
    else if (bIsMap)
    {
        RenderMapXSD( pRequest,
                                 sTypeName,
                                 id >= QMetaType::User );
    }
    else
    {
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        const QMetaObject *pMetaObject = QMetaType::metaObjectForType(id);
#else
        const QMetaObject *pMetaObject = metaType.metaObject();
#endif
        if (pMetaObject)
        {
            QObject* pClass = pMetaObject->newInstance();
            if (pClass != nullptr)
                RenderXSD( pRequest, pClass );
            delete pClass;
        }
    }

    // Create the XML result
    auto data = MythHTTPData::Create(toByteArray());
    data->m_mimeType = MythMimeDatabase::MimeTypeForName("application/xml");
    data->m_cacheType = HTTPETag | HTTPShortLife;
    return MythHTTPResponse::DataResponse(pRequest, data);
}

/////////////////////////////////////////////////////////////////////////////
//
//<?xml version="1.0" encoding="UTF-8"?>
//<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema"
//           xmlns:tns="http://mythtv.org"
//           targetNamespace="http://mythtv.org"
//           elementFormDefault="qualified"
//           attributeFormDefault="unqualified">
//    <xs:include schemaLocation="<path to dependant schema"/>
//
//    <xs:complexType name="<className>">
//        <xs:annotation>
//            <xs:documentation>Comment describing your root element</xs:documentation>
//        </xs:annotation>
//        <xs:sequence>
//            <xs:element minOccurs="0" name="<propName>" type="<propType>"/>
//            <xs:element minOccurs="0" name="<childName>" nillable="true" type="tns:<ChildType>"/>
//        </xs:sequence>
//    </xs:complexType>
//    <xs:element name="<className>" nillable="true" type="tns:<className>"/>
//
//</xs:schema>
/////////////////////////////////////////////////////////////////////////////

bool MythXSD::RenderXSD( HTTPRequest2 pRequest, QObject *pClass )
{
    const QMetaObject *pMetaObject = pClass->metaObject();

    QString     sClassName = ConvertTypeToXSD( pMetaObject->className(), true);
    QDomElement oRoot      = CreateSchemaRoot();

    QMap<QString, TypeInfo> typesToInclude;

    // ------------------------------------------------------------------
    // Create xs:complexType structure
    // ------------------------------------------------------------------

    QDomElement oTypeNode = createElement( "xs:complexType" );
    QDomElement oSeqNode  = createElement( "xs:sequence"    );

    oTypeNode.setAttribute( "name", sClassName );

    oTypeNode.appendChild( oSeqNode  );

    // -=>TODO: Add an xs:annotation node with class descriptions

    // ------------------------------------------------------------------
    // Add all properties for this type
    //
    //  <xs:element minOccurs="0" name="<propName>" type="<propType>"/>
    //    <xs:element minOccurs="0" name="<childName>" nillable="true"
    //              type="tns:<ChildType>"/>
    // ------------------------------------------------------------------

    int nCount = pMetaObject->propertyCount();

    for (int nIdx=0; nIdx < nCount; ++nIdx )
    {
        QMetaProperty metaProperty = pMetaObject->property( nIdx );

        if (metaProperty.isDesignable())
        {
            const char *pszPropName = metaProperty.name();
            QString     sPropName( pszPropName );

            if ( sPropName.compare( "objectName" ) == 0)
                continue;

            // ----------------------------------------------------------
            // Create xs:element for this property
            // ----------------------------------------------------------

            QDomElement oNode        = createElement( "xs:element" );
            QString     sType        = metaProperty.typeName();
            bool        bCustomType  = false;
            QString     sCustomAttr  = "type";
            QString     sContentName = QString();
            QString     sContentType = QString();

            LOG(VB_UPNP, LOG_DEBUG, QString( "Type: %1").arg( sType ));

            // if this is a child object, sType will be QObject*
            // which we can't use, so we need to read the
            // properties value, and read it's metaObject data

            if (sType == "QObject*")
            {
                QVariant val = metaProperty.read( pClass );
                const QObject *pObject = val.value< QObject* >();

                sType = pObject->metaObject()->className();
                bCustomType = true;
            }
            else if ((sType == "QVariantList" ) || (sType == "QVariantMap"))
            {
                sContentType = ReadPropertyMetadata( pClass,
                                                     sPropName,
                                                     "type" );

                if (sContentType.startsWith('Q'))
                    sContentType = sContentType.mid( 1 );

                if (sContentType.startsWith("V2"))
                    sContentType.remove(0,2);
                sContentType.remove( QChar('*'));

                if (sType == "QVariantMap")
                {
                    sContentName = ReadPropertyMetadata( pClass,
                                                         sPropName,
                                                         "name" );

                    if (sContentName.isEmpty())
                        sContentName = sContentType;

                    sType = "MapOfString" + sContentName;
                }
                else
                    sType = "ArrayOf" + sContentType;

                bCustomType = true;

            }
            else if (sType == "QStringList")
            {
                sType = "ArrayOfString";
                bCustomType = true;
            }
            else if (IsEnum( metaProperty, sType ))
            {
                // The code for  creating the enum in xsd does not work
                // Since there is only one enum, treat it as a string
                // sCustomAttr = "enum";
                // if (sType.startsWith("V2"))
                //     sType.remove(0,2);
                // // if sType still contains "::", then no need to prefix with sClassName
                // if (sType.contains( "::" ))
                //     sType = sType.replace( "::", "." );
                // else
                //     sType = sClassName + "." + sType;
                // bCustomType = true;
                sType="string";
            }

            QString sNewPropName( metaProperty.name() );

            if (IsNillable( sType ))
                oNode.setAttribute( "nillable" , static_cast<int>(true) );

            if (bCustomType)
            {
                TypeInfo info = { sCustomAttr, sContentType };
                typesToInclude.insert( sType, info );
            }

            oNode.setAttribute( "type"     , (bCustomType ? "tns:" : "xs:") +
                                       ConvertTypeToXSD( sType, bCustomType ));

            oNode.setAttribute( "name"     , sNewPropName );
            oNode.setAttribute( "minOccurs", 0            );

            oSeqNode.appendChild( oNode );
        }
    }

    // ------------------------------------------------------------------
    // Create element for class
    //
    //       <xs:element name="<className>" nillable="true" type="tns:<className>"/>
    // ------------------------------------------------------------------

    QDomElement oElementNode = createElement( "xs:element" );

    oElementNode.setAttribute( "type"    , "tns:" + sClassName );
    oElementNode.setAttribute( "nillable", "true" );
    oElementNode.setAttribute( "name"    , sClassName );

    // ----------------------------------------------------------------------
    // Build xml tree...
    // ----------------------------------------------------------------------

    appendChild( oRoot );

    if (typesToInclude.count() > 0)
    {
        // ------------------------------------------------------------------
        // Create all needed includes
        //
        //    <xs:include schemaLocation="<path to dependant schema"/>
        // ------------------------------------------------------------------

        // QString sBaseUri = "http://" + pRequest->GetLastHeader( "host" ) +
        //                                pRequest->m_sResourceUrl;
        QString sBaseUri("http://" +
                                    pRequest->m_headers->value("host") +
                                    pRequest->m_path + "xsd");
        QMap<QString, TypeInfo >::const_iterator it = typesToInclude.constBegin();
        while( it != typesToInclude.constEnd())
        {
            QDomElement oIncNode = createElement( "xs:include" );
            QString     sType    = it.key();

            if (sType.startsWith("V2"))
                sType.remove(0,2);

            TypeInfo info = it.value();

            QString sValue = QString( "%1?%2=%3" ).arg( sBaseUri,
                                                        info.sAttrName,
                                                        sType );

            if (!info.sContentType.isEmpty())
                sValue += "&name=" + info.sContentType;

            oIncNode.setAttribute( "schemaLocation", sValue );

            oRoot.appendChild( oIncNode );
            ++it;
        }
    }

    oRoot.appendChild( oTypeNode    );
    oRoot.appendChild( oElementNode );

    // ----------------------------------------------------------------------
    // Return xsd doc to caller
    // ----------------------------------------------------------------------

    // QTextStream os( &(pRequest->m_response) );

    // pRequest->m_eResponseType   = ResponseTypeXML;

    // save( os, 0 );

    return true;
}

bool MythXSD::IsEnum( const QMetaProperty &metaProperty, const QString &/*sType*/ )
{
    return (metaProperty.isEnumType() || metaProperty.isFlagType() );
}

/////////////////////////////////////////////////////////////////////////////
//
// ArrayOf<Type>
//
//<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:tns="http://mythtv.org" targetNamespace="http://mythtv.org" elementFormDefault="qualified" attributeFormDefault="unqualified">
//    <xs:include schemaLocation="<type>.xsd"/>
//
//    <xs:complexType name="ArrayOf<Type>">
//        <xs:annotation>
//            <xs:documentation>Comment describing your root element</xs:documentation>
//        </xs:annotation>
//        <xs:sequence>
//            <xs:element minOccurs="0" maxOccurs="unbounded" name="<type>" nillable="true" type="tns:<Type>"/>
//        </xs:sequence>
//    </xs:complexType>
//    <xs:element name="ArrayOf<Type>" nillable="true" type="tns:ArrayOf<Type>"/>
//
//</xs:schema>
//
/////////////////////////////////////////////////////////////////////////////

bool MythXSD::RenderArrayXSD( HTTPRequest2 pRequest,
                          const QString &sClassName,
                          bool           bCustomType )
{
    QString     sArrayName = "ArrayOf" + sClassName;
    QString     sType;

    if (bCustomType)
        sType = "tns:" + sClassName;
    else
        sType = "xs:" + ConvertTypeToXSD( sClassName, false );

    QDomElement oRoot      = CreateSchemaRoot();

    // ------------------------------------------------------------------
    // Create xs:complexType structure
    // ------------------------------------------------------------------

    QDomElement oTypeNode = createElement( "xs:complexType" );
    QDomElement oSeqNode  = createElement( "xs:sequence"    );

    // -=>TODO: Add an xs:annotation node with class descriptions

    oTypeNode.setAttribute( "name", sArrayName );
    oTypeNode.appendChild( oSeqNode  );

    QDomElement oNode = createElement( "xs:element" );

    oNode.setAttribute( "type"      , sType );
    oNode.setAttribute( "nillable"  , "true" );
    oNode.setAttribute( "name"      , sClassName );
    oNode.setAttribute( "maxOccurs" , "unbounded" );
    oNode.setAttribute( "minOccurs" , "0"         );

    oSeqNode.appendChild( oNode  );

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------

    QDomElement oElementNode = createElement( "xs:element" );

    oElementNode.setAttribute( "type"    , "tns:" + sArrayName );
    oElementNode.setAttribute( "nillable", "true" );
    oElementNode.setAttribute( "name"    , sArrayName );

    // ----------------------------------------------------------------------
    // Build xml tree...
    // ----------------------------------------------------------------------

    appendChild( oRoot );

    if (bCustomType)
    {
        QDomElement oIncNode = createElement( "xs:include" );

        // QString sBaseUri = "http://" + pRequest->GetLastHeader( "host" ) +
        //                                pRequest->m_sResourceUrl + "?type=";
        QString sBaseUri("http://" +
                                    pRequest->m_headers->value("host") +
                                    pRequest->m_path + "xsd" + "?type=");

        oIncNode.setAttribute( "schemaLocation", sBaseUri + sClassName );

        oRoot.appendChild( oIncNode );
    }

    oRoot.appendChild( oTypeNode    );
    oRoot.appendChild( oElementNode );

    // ----------------------------------------------------------------------
    // Return xsd doc to caller
    // ----------------------------------------------------------------------

    // QTextStream os( &(pRequest->m_response) );

    // pRequest->m_eResponseType   = ResponseTypeXML;

    // save( os, 0 );

    return true;

}

/////////////////////////////////////////////////////////////////////////////
//
// MapOfString<Type>
//
//<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:tns="http://mythtv.org" targetNamespace="http://mythtv.org" elementFormDefault="qualified" attributeFormDefault="unqualified">
//    <xs:include schemaLocation="<type>.xsd"/>
//
//<xs:complexType name="MapOfString<Type>">
//  <xs:annotation>
//    <xs:appinfo>
//      <IsDictionary xmlns="http://schemas.microsoft.com/2003/10/Serialization/">true</IsDictionary>
//    </xs:appinfo>
//  </xs:annotation>
//  <xs:sequence>
//    <xs:element minOccurs="0" maxOccurs="unbounded" name="KeyValueOfString<Type>">
//      <xs:complexType>
//        <xs:sequence>
//           <xs:element name="Key" nillable="true" type="xs:string" />
//           <xs:element name="Value" nillable="true" type="<Type>"  />
//        </xs:sequence>
//      </xs:complexType>
//    </xs:element>
//  </xs:sequence>
//</xs:complexType>
//<xs:element name="MapOfString<Type>" nillable="true" type="tns:MapOfString<Type>" />
//
//</xs:schema>
//
/////////////////////////////////////////////////////////////////////////////

bool MythXSD::RenderMapXSD( HTTPRequest2 pRequest,
                        const QString &sClassName,
                        bool           bCustomType )
{
    QString     sArrayName   = "MapOfString" + sClassName;
    // QString     sMapItemName = pRequest->m_mapParams[ "name" ];
    QString     sMapItemName = pRequest->m_queries.value("name");

    QString     sType;

    if (sMapItemName.isEmpty())
        sMapItemName = sClassName;

    if (bCustomType)
        sType = "tns:" + sMapItemName;
    else
        sType = "xs:" + ConvertTypeToXSD( sMapItemName, false );

    QDomElement oRoot      = CreateSchemaRoot();

    // ------------------------------------------------------------------
    // Create xs:complexType structure
    // ------------------------------------------------------------------

    QDomElement oTypeNode = createElement( "xs:complexType" );
    QDomElement oSeqNode  = createElement( "xs:sequence"    );
    QDomElement oAnno     = createElement( "xs:annotation"  );
    QDomElement oAppInfo  = createElement( "xs:appinfo"     );

    QDomElement oNode = createElement( "IsDictionary" );
    oNode.setAttribute( "xmlns", "http://schemas.microsoft.com/2003/10/Serialization/" );
    oNode.appendChild( createTextNode( "true" ));

    oTypeNode.appendChild( oAnno    );
    oAnno    .appendChild( oAppInfo );
    oAppInfo .appendChild( oNode    );

    // -=>TODO: Add an xs:annotation node with class descriptions

    // ----------------------------------------------------------------------
    //  <xs:sequence>
    // ----------------------------------------------------------------------

    oTypeNode.setAttribute( "name", sArrayName );
    oTypeNode.appendChild( oSeqNode  );

    // ----------------------------------------------------------------------
    //    <xs:element minOccurs="0" maxOccurs="unbounded" name="KeyValueOfString<Type>">
    // ----------------------------------------------------------------------

    QDomElement oInnerNode = createElement( "xs:element" );

    oInnerNode.setAttribute( "name"      , sClassName  );
    oInnerNode.setAttribute( "maxOccurs" , "unbounded" );
    oInnerNode.setAttribute( "minOccurs" , "0"         );

    oSeqNode.appendChild( oInnerNode );

    // ----------------------------------------------------------------------
    //      <xs:complexType>
    // ----------------------------------------------------------------------
    oNode = createElement( "xs:complexType" );

    oInnerNode.appendChild( oNode );

    // ----------------------------------------------------------------------
    //        <xs:sequence>
    // ----------------------------------------------------------------------

    QDomElement oInnerSeq = createElement( "xs:sequence" );

    oNode.appendChild( oInnerSeq );

    // ----------------------------------------------------------------------
    //           <xs:element name="Key" nillable="true" type="xs:string" />
    // ----------------------------------------------------------------------

    oNode = createElement( "xs:element" );

    oNode.setAttribute( "type"      , "xs:string" );
    oNode.setAttribute( "nillable"  , "true" );
    oNode.setAttribute( "name"      , "Key" );

    oInnerSeq.appendChild( oNode  );

    // ----------------------------------------------------------------------
    //           <xs:element name="Value" nillable="true" type="<Type>"  />
    // ----------------------------------------------------------------------

    oNode = createElement( "xs:element" );

    oNode.setAttribute( "type"      , sType );
    oNode.setAttribute( "nillable"  , "true" );
    oNode.setAttribute( "name"      , "Value" );

    oInnerSeq.appendChild( oNode  );

    // ----------------------------------------------------------------------
    //<xs:element name="MapOfString<Type>" nillable="true" type="tns:MapOfString<Type>" />
    // ----------------------------------------------------------------------

    QDomElement oElementNode = createElement( "xs:element" );

    oElementNode.setAttribute( "type"    , "tns:" + sArrayName );
    oElementNode.setAttribute( "nillable", "true" );
    oElementNode.setAttribute( "name"    , sArrayName );

    // ----------------------------------------------------------------------
    // Build xml tree...
    // ----------------------------------------------------------------------

    appendChild( oRoot );

    if (bCustomType)
    {
        QDomElement oIncNode = createElement( "xs:include" );

        // QString sBaseUri = "http://" + pRequest->GetLastHeader( "host" ) + pRequest->m_sResourceUrl + "?type=";
        QString sBaseUri("http://" +
                                    pRequest->m_headers->value("host") +
                                    pRequest->m_path + "xsd" + "?type=");

        oIncNode.setAttribute( "schemaLocation", sBaseUri + sClassName );

        oRoot.appendChild( oIncNode );
    }

    oRoot.appendChild( oTypeNode    );
    oRoot.appendChild( oElementNode );

    // ----------------------------------------------------------------------
    // Return xsd doc to caller
    // ----------------------------------------------------------------------

    // QTextStream os( &(pRequest->m_response) );

    // pRequest->m_eResponseType   = ResponseTypeXML;

    // save( os, 0 );

    return true;

}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

QDomElement MythXSD::CreateSchemaRoot()
{
    QString sTargetNamespace = "http://mythtv.org";

    QDomElement oRoot = createElement( "xs:schema" );

    oRoot.setAttribute( "xmlns:xs"            , "http://www.w3.org/2001/XMLSchema");
    oRoot.setAttribute( "xmlns:tns"           , sTargetNamespace );
    oRoot.setAttribute( "targetNamespace"     , sTargetNamespace );
    oRoot.setAttribute( "elementFormDefault"  , "qualified" );
    oRoot.setAttribute( "attributeFormDefault", "unqualified" );
    return oRoot;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

bool MythXSD::IsNillable( const QString &sType )
{
    if (sType.startsWith( "V2"))
        return true;

    if (sType == "QDateTime")
        return true;

    if (sType == "QDate")
        return true;

    if (sType == "QTime")
        return true;

    return false;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

QString MythXSD::ConvertTypeToXSD( const QString &sType, bool bCustomType )
{
    if (bCustomType || sType.startsWith( "V2"))
    {
        QString sTypeName( sType );

        if (sTypeName.startsWith("V2"))
            sTypeName.remove(0,2);
        sTypeName.remove( QChar('*') );

        if (sType == "QStringList" )
            return "ArrayOfString";

        return sTypeName;
    }

    if (sType == "QDateTime")
        return "dateTime";

    if (sType == "QDate")
        return "date";

    if (sType == "QTime")
        return "time";

    if (sType == "bool")
        return "boolean";

    if (sType == "uint")
        return "unsignedInt";

    if (sType == "qlonglong" )
        return "long";

    if (sType == "std::chrono::seconds" )
        return "long";

    if (sType == "qulonglong" )
        return "unsignedLong";

    if (sType == "QJsonObject" )
        return "string";

    if (sType == "long long" )
        return "long";

    if (sType == "QFileInfo" )
        return "string";        // temp solution

    if (sType.startsWith('Q'))
        return sType.mid( 1 ).toLower();

    return sType.toLower();
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

QString MythXSD::ReadPropertyMetadata( QObject *pObject, const QString& sPropName, const QString& sKey )
{
    const QMetaObject *pMeta = pObject->metaObject();

    int nIdx = -1;

    if (pMeta)
        nIdx = pMeta->indexOfClassInfo( sPropName.toUtf8() );

    if (nIdx >=0)
    {
        QString     sMetadata = pMeta->classInfo( nIdx ).value();
        QStringList sOptions  = sMetadata.split( ';' );

        QString     sFullKey  = sKey + "=";

        for (const QString& option : qAsConst(sOptions))
        {
            if (option.startsWith( sFullKey ))
                return option.mid( sFullKey.length() );
        }
    }

    return QString();
}

HTTPResponse MythXSD::Error(HTTPRequest2 pRequest, const QString &msg)
{
    LOG(VB_GENERAL, LOG_ERR, "MythCSD Exception: " + msg);
    pRequest->m_status = HTTPBadRequest;
    return MythHTTPResponse::ErrorResponse(pRequest, msg);
}