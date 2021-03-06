///////////////////////////////////////////////////////////////////////////////
//
// CoinSocket
//
// channels.h
//
// Copyright (c) 2014 Eric Lombrozo
//
// All Rights Reserved.
//

#pragma once

#include <set>
#include <string>
#include <map>

namespace CoinSocket
{

typedef std::set<std::string> Channels;
typedef std::multimap<std::string, std::string> ChannelSets;
typedef std::pair<std::string, std::string> ChannelSetItem;
typedef std::pair<ChannelSets::iterator, ChannelSets::iterator> ChannelRange;

const Channels&     getChannels();
void                addChannel(const std::string& channel);
bool                channelExists(const std::string& channel);

const ChannelSets&  getChannelSets();
void                addChannelToSet(const std::string& channelSet, const std::string& channel);
ChannelRange        getChannelRange(const std::string& channelSet);
bool                isChannelRangeEmpty(const ChannelRange& range);

}
