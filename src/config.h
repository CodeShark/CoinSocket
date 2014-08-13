///////////////////////////////////////////////////////////////////////////////
//
// CoinSocket
//
// config.h
//
// Copyright (c) 2014 Eric Lombrozo
//
// All Rights Reserved.
//

#pragma once

#include "CoinSocketExceptions.h"
#include <CoinQ/CoinQ_coinparams.h>

#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <sysutils/filesystem.h>

const std::string DEFAULT_DATADIR = "CoinSocket";
const std::string DEFAULT_DOCDIR = "vaultdocs";
const std::string DEFAULT_CONFIG_FILE = "coinsocket.conf";
const std::string DEFAULT_PEER_HOST = "localhost";
const std::string DEFAULT_WEBSOCKET_PORT = "8080";
const std::string DEFAULT_ALLOWED_IPS = "^\\[(::1|::ffff:127\\.0\\.0\\.1)\\].*";

class CoinSocketConfig
{
public:
    CoinSocketConfig() : m_bHelp(false) { }

    void init(int argc, char* argv[]);

    const std::string& getConfigFile() const { return m_configFile; }
    const std::string& getNetworkName() const { return m_networkName; }
    const std::string& getDatabaseUser() const { return m_databaseUser; }
    const std::string& getDatabasePassword() const { return m_databasePassword; }
    const std::string& getDatabaseName() const { return m_databaseName; }
    const std::string& getDataDir() const { return m_dataDir; }
    const std::string& getDocumentDir() const { return m_documentDir; }
    bool               getSync() const { return m_bSync; }
    const std::string& getPeerHost() const { return m_peerHost; }
    const std::string& getPeerPort() const { return m_peerPort; }
    const std::string& getWebSocketPort() const { return m_webSocketPort; }
    const std::string& getAllowedIps() const { return m_allowedIps; }
    const std::string& getConnectKey() const { return m_connectKey; }
    const std::string& getTlsCertificateFile() const { return m_tlsCertificateFile; }
    const CoinQ::CoinParams& getCoinParams() const { return m_networkSelector.getCoinParams(); }

    bool help() const { return m_bHelp; }
    const std::string& getHelpOptions() const { return m_helpOptions; }

private:
    CoinQ::NetworkSelector m_networkSelector;

    std::string m_configFile;
    std::string m_networkName;
    std::string m_databaseUser;
    std::string m_databasePassword;
    std::string m_databaseName;
    std::string m_dataDir;
    std::string m_documentDir;
    bool        m_bSync;
    std::string m_peerHost;
    std::string m_peerPort;
    std::string m_webSocketPort;
    std::string m_allowedIps;
    std::string m_connectKey;
    std::string m_tlsCertificateFile;

    bool m_bHelp;
    std::string m_helpOptions;
};

inline void CoinSocketConfig::init(int argc, char* argv[])
{
    namespace po = boost::program_options;
    po::options_description options("Options");
    options.add_options()
        ("help", "display help message")
        ("config", po::value<std::string>(&m_configFile), "name of the configuration file")
        ("network", po::value<std::string>(&m_networkName), "name of the p2p network")
        ("dbuser", po::value<std::string>(&m_databaseUser), "vault database user")
        ("dbpasswd", po::value<std::string>(&m_databasePassword), "vault database password")
        ("dbname", po::value<std::string>(&m_databaseName), "vault database name")
        ("datadir", po::value<std::string>(&m_dataDir), "data directory")
        ("docdir", po::value<std::string>(&m_documentDir), "document directory")
        ("sync", po::value<bool>(&m_bSync), "set to true to turn on p2p synchronization")
        ("peerhost", po::value<std::string>(&m_peerHost), "peer hostname")
        ("peerport", po::value<std::string>(&m_peerPort), "peer port")
        ("wsport", po::value<std::string>(&m_webSocketPort), "port to listen for inbound websocket connections")
        ("allowedips", po::value<std::string>(&m_allowedIps), "regular expression for allowed ip addresses")
        ("connectkey", po::value<std::string>(&m_connectKey), "key to be supplied when connecting")
        ("tlscertfile", po::value<std::string>(&m_tlsCertificateFile), "TLS certificate file")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, options), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        m_bHelp = true;
        std::stringstream ss;
        ss << options;
        m_helpOptions = ss.str();
        return;
    }

    std::string datadir(DEFAULT_DATADIR);
    if (vm.count("network"))
    {
        std::transform(m_networkName.begin(), m_networkName.end(), m_networkName.begin(), ::tolower);
        datadir += "_";
        datadir += m_networkName; 
    }

    using namespace sysutils::filesystem;
    if (!vm.count("datadir"))       { m_dataDir = getDefaultDataDir(datadir); }
    if (!vm.count("config"))        { m_configFile =  m_dataDir + "/" + DEFAULT_CONFIG_FILE; }

    namespace fs = boost::filesystem;
    fs::path dataDirPath(m_dataDir);
    if ((!fs::exists(dataDirPath) && !fs::create_directory(dataDirPath)) || !fs::is_directory(dataDirPath))
        throw CoinSocket::ConfigInvalidDataDirException();

    fs::path p(m_configFile);
    if (fs::exists(p))
    {
        std::ifstream config(m_configFile);
        po::store(po::parse_config_file(config, options), vm);
        config.close();
        po::notify(vm);     
    }

    if (!vm.count("network")) throw CoinSocket::ConfigMissingNetworkException();
    m_networkSelector.select(m_networkName);

    if (!vm.count("dbname")) throw CoinSocket::ConfigMissingDBNameException(); 
    if (!vm.count("docdir"))        { m_documentDir = getUserProfileDir() + "/" + DEFAULT_DOCDIR; }
    if (!vm.count("sync"))          { m_bSync = false; }
    if (!vm.count("peerhost"))      { m_peerHost = DEFAULT_PEER_HOST; }
    if (!vm.count("peerport"))      { m_peerPort = m_networkSelector.getCoinParams().default_port(); }
    if (!vm.count("wsport"))        { m_webSocketPort = DEFAULT_WEBSOCKET_PORT; }
    if (!vm.count("allowedips"))    { m_allowedIps = DEFAULT_ALLOWED_IPS; }
}

