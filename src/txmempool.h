// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef BITCOIN_TXMEMPOOL_H
#define BITCOIN_TXMEMPOOL_H

#include <list>
#include <map>

//#include "coins.h"
#include "core.h"
#include "sync.h"
#include <memory>

using namespace std;
/** Fake height value used in CCoins to signify they are only in the memory pool (since 0.8) */
static const unsigned int MEMPOOL_HEIGHT = 0x7FFFFFFF;

class CAccountViewCache;

/*
 * CTxMemPool stores these:
 */
class CTxMemPoolEntry
{
private:
	std::shared_ptr<CBaseTransaction> pTx;
    int64_t nFee; // Cached to avoid expensive parent-transaction lookups
    size_t nTxSize; // ... and avoid recomputing tx size
    int64_t nTime; // Local time when entering the mempool
    double dPriority; // Priority when entering the mempool
    unsigned int nHeight; // Chain height when entering the mempool

public:
    CTxMemPoolEntry(CBaseTransaction *ptx, int64_t _nFee,
                    int64_t _nTime, double _dPriority, unsigned int _nHeight);
    CTxMemPoolEntry();
    CTxMemPoolEntry(const CTxMemPoolEntry& other);

    std::shared_ptr<CBaseTransaction> GetTx() const { return pTx;}
    double GetPriority(unsigned int currentHeight) const;
    int64_t GetFee() const { return nFee; }
    size_t GetTxSize() const { return nTxSize; }
    int64_t GetTime() const { return nTime; }
    unsigned int GetHeight() const { return nHeight; }
};

/*
 * CTxMemPool stores valid-according-to-the-current-best-chain
 * transactions that may be included in the next block.
 *
 * Transactions are added when they are seen on the network
 * (or created by the local node), but not all transactions seen
 * are added to the pool: if a new transaction double-spends
 * an input of a transaction in the pool, it is dropped,
 * as are non-standard transactions.
 */
class CTxMemPool
{
private:
    bool fSanityCheck; // Normally false, true if -checkmempool or -regtest
    unsigned int nTransactionsUpdated;  //TODO meaning
public:
    mutable CCriticalSection cs;
    map<uint256, CTxMemPoolEntry > mapTx;
    std::shared_ptr<CAccountViewCache> pAccountViewCache;

    CTxMemPool();

    void setSanityCheck(bool _fSanityCheck) { fSanityCheck = _fSanityCheck; }

    bool addUnchecked(const uint256& hash, const CTxMemPoolEntry &entry);

    void remove(CBaseTransaction *pBaseTx, list<std::shared_ptr<CBaseTransaction> >& removed, bool fRecursive = false);

    void removeConflicts(CBaseTransaction *pBaseTx, list<std::shared_ptr<CBaseTransaction> >& removed);
    void clear();
    void queryHashes(vector<uint256>& vtxid);
    void pruneSpent(const uint256& hash, CCoins &coins);
    unsigned int GetTransactionsUpdated() const;
    void AddTransactionsUpdated(unsigned int n);

    unsigned long size()
    {
        LOCK(cs);
        return mapTx.size();
    }

    bool exists(uint256 hash)
    {
        LOCK(cs);
		return ((mapTx.count(hash) != 0));
    }

    std::shared_ptr<CBaseTransaction> lookup(uint256 hash) const;

    void SetAccountViewDB(CAccountViewCache *pAccountViewCacheIn);
};


#endif /* BITCOIN_TXMEMPOOL_H */