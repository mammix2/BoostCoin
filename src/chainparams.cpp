// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"
#include "consensus/consensus.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "base58.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << CScriptNum(0) << CScriptNum(42) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = 0;
    txNew.vout[0].scriptPubKey.clear();

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.vtx[0].nTime = nTime;
    genesis.vtx[0].UpdateHash();
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion)
{
    const char* pszTimestamp = "x13 BoostCoin to Mars!... Again!";
    return CreateGenesisBlock(pszTimestamp, nTime, nNonce, nBits, nVersion);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 16 * 60; // 16 minutes
        consensus.nPowTargetSpacing = 60;
        consensus.nPowTargetSpacing2 = 2 * 60;
		consensus.nPowTargetSpacing2_start = 640450;		
        consensus.nLastPOWBlock = 10000000;
        consensus.nStakeMinAge = 24 * 60 * 60;
		consensus.nBlockVersionCheckStart = 43201; //m2: blocks after this will be version checked.
		consensus.nBlockDGW_start = 250;
        consensus.nModifierInterval = 10 * 60;
        consensus.nStakeSplitAge = 24 * 60 * 60;
		consensus.nTimeDriftCondition1 = 1457136000; // Sat, 05 Mar 2016 00:00:00 GMT
		consensus.nTimeDriftCondition2 = 1461873600; // Thu, 28 Apr 2016 20:00:00 GMT
        consensus.nStakeCombineThreshold = 1000 * COIN;
        consensus.checkpointPubKey = "04b7640b474644ccc5f5ca40ae97f06261e4650ef2f59f38e03cc48e02f16a1b0ce6bb45798e335c84311919c934d0f7866594a249a83be71405e63783ad9d1b12";
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 15120; // 95% of 20160
        consensus.nMinerConfirmationWindow = 20160; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1533081600; // August 1st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1564617600; // August 1st, 2019

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1533081600; // August 1st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1564617600; // August 1st, 2019

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000100010");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x70;
        pchMessageStart[1] = 0x35;
        pchMessageStart[2] = 0x22;
        pchMessageStart[3] = 0x05;
        nDefaultPort = 9697;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1400512373, 5726282, 0x1e0fffff, 1);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000e81cae8e8cbbf9679d53ff1df5e23062dc667a332fd121f86e68f5d25e3"));
        assert(genesis.hashMerkleRoot == uint256S("0x603add6547a4c67f12f5a5fb5100fb455e3515b05e58a44a29a4496f7fcaaaf7"));

        // Note that of those with the service bits flag, most only support a subset of possible options
		vSeeds.push_back(CDNSSeedData("ns1.bost.link", "myboost001.bost.link", false));
		vSeeds.push_back(CDNSSeedData("ns2.bost.link", "myboost002.bost.link", false));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,25);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,85);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,153);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (0, uint256S("0x00000e81cae8e8cbbf9679d53ff1df5e23062dc667a332fd121f86e68f5d25e3"))
            (100, uint256S ("0x000000ff3b169878019e2ef990113cb47dfe4b92d18f49ac0233011b89d0ae3b"))
            (1000, uint256S ("0x00000000087645f692b29da415d7cc55e0d0925ed1492c4703c2c4b634b8f29b"))
            (5002, uint256S ("0x0000000007499ff2a0bad402934236edb780872b3c3defe5f1612981b2307c8b"))
            (10000, uint256S ("0x000000000d0d85fcccc2c8e1e36ade1427006ec7ebdac9f9d7499c11251bb1fc"))
            (15001, uint256S ("0x000000000470415c747542f89ff3aaeb0e5387f0132035c9370fe7503796ca28"))
            (25002, uint256S ("0x00000000003d118a65638737be61aba2f8ff9f655d60153bb3c8607003f73e10"))
            (30001, uint256S ("0x000000000065dfe36520036c8c414baf9986a948fe9f793437f5e45c9c4ccd68"))
            (140000, uint256S ("0x20cc8a57580d99c07f2f3c77aab95f54ead88feb6a1d292f7eb51854944e0256"))
            (156000, uint256S ("0xc9c0e2e598da65a7a2036586403003f9994ccfaa7906fb2caaab5a50361ce049"))
            (306500, uint256S ("0xd509595cfd52380ac4059e8baedd01e3ac89694279c5fd79d2693308cf8ffa3e"))
            (489550, uint256S ("0x9801f200e470a13aca3c3fd54f0a17e5945eea24b378c535a008835af527beb0"))
            (600000, uint256S ("0x9dcdc9959bb6d16f6fc852f8a77828779d265d45c52ec216f6a7d235a689ee1d"))
            (811300, uint256S ("0x000002539bf0373bd1adc04e30fcb9ae5f46f27694d3bdbbdd6b0195e709db5c"))
            (884400, uint256S ("0x00000c0d2c9d2d326ca6413eb362f8576366f4559837006f88da773ebf556fc2"))
            (900000, uint256S ("0x0000028d002d0e42a0aefae9eb7d526fc1e6a212f2af90fb893daf95dad0c729"))
            (930500, uint256S ("0x000005de51c6c8cbd82b480195f6ac9665c5e7963b9531f7c6a7bf077443050f"))
            (1319500, uint256S ("0x6ae145bba3b4eb2b15e6dc98f43f4d70eafa981150c8669db2d0cf87c1b7987e"))

            ,
            1527684187, // * UNIX timestamp of last checkpoint block
            1319501,   // * total number of transactions between genesis and last checkpoint //m2 needs verifying
                        //   (the tx=... number in the SetBestChain debug.log lines)
            720     // * estimated number of transactions per day after checkpoint //m2 needs verifying
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nMajorityEnforceBlockUpgrade = 50;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 16 * 60; // 16 minutes
        consensus.nPowTargetSpacing = 60;
        consensus.nPowTargetSpacing2 = 2 * 60;	
		consensus.nPowTargetSpacing2_start = 100;		
        consensus.nLastPOWBlock = 150000;
        consensus.nStakeMinAge = 15 * 60; // testnet min age is 15 mins
        consensus.nModifierInterval = 60;
        consensus.nStakeSplitAge = 24 * 60 * 60;
		consensus.nTimeDriftCondition1 = 1457136000; // Sat, 05 Mar 2016 00:00:00 GMT
		consensus.nTimeDriftCondition2 = 1461873600; // Thu, 28 Apr 2016 20:00:00 GMT
        consensus.nBlockVersionCheckStart = 5; //m2: blocks after this will be version checked.
		consensus.nBlockDGW_start = 20;
        consensus.nStakeCombineThreshold = 1000 * COIN;
        consensus.checkpointPubKey = "04f80fc7adbebe2dd223f0ad811dee52ffd4edfdeff1a1b08ccdbbbba82eda6c2917c0bdecc3c5e855652111b5b220c679b061c9bcf330e9534e7bc006bd45673e";
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1537315200; // Wednesday, 19 September 2018 01:00:00 GMT+01:00
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1568851200; // Wednesday, 19 September 2019 01:00:00 GMT+01:00

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1537315200; // Wednesday, 19 September 2018 01:00:00 GMT+01:00
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1568851200; // Wednesday, 19 September 2019 01:00:00 GMT+01:00

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000100010");

        pchMessageStart[0] = 0x70;
        pchMessageStart[1] = 0x35;
        pchMessageStart[2] = 0x22;
        pchMessageStart[3] = 0x05;
        nDefaultPort = 19697;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1400512373, 5726282, 0x1e0fffff, 1);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000e81cae8e8cbbf9679d53ff1df5e23062dc667a332fd121f86e68f5d25e3"));
        assert(genesis.hashMerkleRoot == uint256S("0x603add6547a4c67f12f5a5fb5100fb455e3515b05e58a44a29a4496f7fcaaaf7"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("test-myboost001.bost.link", "test-myboost001.bost.link", false));
		vSeeds.push_back(CDNSSeedData("test-myboost002.bost.link", "test-myboost002.bost.link", false));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;


        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x00000e81cae8e8cbbf9679d53ff1df5e23062dc667a332fd121f86e68f5d25e3")),
            1400512373,  //m2 needs verifying
            1,
            720  //m2 needs verifying
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 16 * 60; // 16 minutes
        consensus.nPowTargetSpacing = 60;
        consensus.nPowTargetSpacing2 = 2 * 60;
		consensus.nPowTargetSpacing2_start = 30;
        consensus.nStakeMinAge = 24 * 60 * 60;
        consensus.nModifierInterval = 10 * 60;
        consensus.nStakeSplitAge = 24 * 60 * 60;
		consensus.nTimeDriftCondition1 = 1457136000; // Sat, 05 Mar 2016 00:00:00 GMT
		consensus.nTimeDriftCondition2 = 1461873600; // Thu, 28 Apr 2016 20:00:00 GMT
        consensus.nBlockVersionCheckStart = 100; //m2: blocks after this will be version checked.
		consensus.nBlockDGW_start = 20;
        consensus.nStakeCombineThreshold = 1000 * COIN;
        consensus.checkpointPubKey = "04911c06093afaa119c95209c5f993dd855169f1972a73e08ffd3947826488bd42b9155ff7353016f1a2e4d5fe4e78c3d25310eea86e1132c7bbcd9b072c380428";
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000100010");

        pchMessageStart[0] = 0x41;
        pchMessageStart[1] = 0x32;
        pchMessageStart[2] = 0x23;
        pchMessageStart[3] = 0x14;
        nDefaultPort = 29697;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1400512373, 825214, 0x1e0fffff, 1);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000ec4fc2a161ae8fc6090888b34f8f88a8f600270affb084c7004b980584c"));
        assert(genesis.hashMerkleRoot == uint256S("0x603add6547a4c67f12f5a5fb5100fb455e3515b05e58a44a29a4496f7fcaaaf7"));


        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x00000ec4fc2a161ae8fc6090888b34f8f88a8f600270affb084c7004b980584c")),
            0,
            0,
            0
        };
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}

CScript CChainParams::GetRewardScript() const {

    CBitcoinAddress address;
    if (Params().NetworkIDString() == CBaseChainParams::MAIN)
        address = CBitcoinAddress("BKqAh5ojyS7bkjaDHJEWXxMwKNUvUsNZak"); //keep this mainnet address BKqAh5ojyS7bkjaDHJEWXxMwKNUvUsNZak
    else if (Params().NetworkIDString() == CBaseChainParams::TESTNET)
        address = CBitcoinAddress("miiWpmBt9bMknbcoe4beokur4xAetJ9Nmz"); 
    else if (Params().NetworkIDString() == CBaseChainParams::REGTEST)
        address = CBitcoinAddress("qKn4qdApT3ARHf2s6NhDyeSy9569Xb3GyS");

    assert(address.IsValid());
    return GetScriptForDestination(address.Get());
}
