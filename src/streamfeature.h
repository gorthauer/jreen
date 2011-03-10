/****************************************************************************
 *  streamfeature.h
 *
 *  Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>
 *
 ***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#ifndef STREAMFEATURE_H
#define STREAMFEATURE_H

#include <QObject>
#include "jid.h"

class QXmlStreamWriter;
class QXmlStreamAttributes;

namespace Jreen
{

class DataStream;
class Client;

class JREEN_EXPORT StreamInfo
{
public:
	enum CompletedFlag
	{
		ResendHeader = 1,
		Authorized = 2,
		ActivateNext = 4,
		Connected = 8,
		AuthorizationFailed = 16
	};
	Q_DECLARE_FLAGS(CompletedFlags, CompletedFlag)

	virtual ~StreamInfo() {}
	virtual QString streamID() = 0;
	virtual QString connectionServer() = 0;
	virtual JID jid() = 0;
	virtual QString password() = 0;
	virtual Client *client() = 0;
	virtual QXmlStreamWriter *writer() = 0;
	virtual void completed(const CompletedFlags &flags = ResendHeader) = 0;
	virtual void setJID(const JID &jid) = 0;
	virtual void addDataStream(DataStream *data_stream) = 0;
};

class JREEN_EXPORT StreamFeature : public XmlStreamParser
{
	Q_DISABLE_COPY(StreamFeature)
public:
	enum Type
	{
		SecurityLayer,
		CompressionLayer,
		SASL,
		SimpleAuthorization,
		Custom,
		Invalid
	};
	StreamFeature(Type type) : m_info(0), m_client(0), m_type(type) {}
	virtual ~StreamFeature() {}
	virtual int priority() = 0;
	virtual void setStreamInfo(StreamInfo *info) { if(info) m_client = (m_info = info)->client(); else { m_info = 0; m_client = 0; } }
	virtual void reset() {}
	virtual bool isActivatable() = 0;
	virtual bool activate() = 0;
	inline Type type() const { return m_type; }
protected:
	StreamInfo *m_info;
	Client *m_client;
	inline void completed(const StreamInfo::CompletedFlags &flags = StreamInfo::ResendHeader) { if(m_info) m_info->completed(flags); }
	inline void addDataStream(DataStream *data_stream) { if(m_info) m_info->addDataStream(data_stream); }
private:
	const Type m_type;
};

}

Q_DECLARE_OPERATORS_FOR_FLAGS(Jreen::StreamInfo::CompletedFlags)
Q_DECLARE_INTERFACE(Jreen::StreamFeature,"org.qutim.JReen.StreamFeature");

#endif // STREAMFEATURE_H
