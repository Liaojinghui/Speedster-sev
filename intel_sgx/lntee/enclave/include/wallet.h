//
// Created by nuanbing on 5/23/19.
//

#ifndef EVM_BALANCE_H
#define EVM_BALANCE_H

#define OE_BUILD_ENCLAVE

#ifdef _PTHREAD_ENC_
#  include "thread.h"
#endif

#include <iostream>
#include <map>
#include <openenclave/edger8r/enclave.h>
#include <openenclave/enclave.h>
#include "eEVM/common.h"
#include "eEVM/simple/simpleglobalstate.h"
#include "message.h"

#include <atomic>

class Wallet {
private:
    eevm::SimpleAccount *account;
    struct Initial_State initial_state;
    uint256_t valid_balance;
    std::map <eevm::Instance_id, uint32_t>
            lock_list; // Credit been locked into the contract
    std::map <eevm::Pubkey, uint32_t> spent; // directly// contract  spent credits
    // (conrtact instance id == pubkey)
    std::map <eevm::Pubkey, uint32_t> receive; // Received credits
    static Wallet *m_pInstance;
public:
    Wallet();

    ~Wallet();

    static Wallet *Instance();

    bool in_cache(std::string addr);

    eevm::SimpleAccount *get_account() {
        return this->account;
    };

    /**
     * Set the valid balance, for test only
     *
     * @param bal: balance
     */
    void set_balance(uint256_t bal);

    /**
     * Initialize/update the initial state of TEE account
     *
     * @param tx_list: the list of tx;
     * @param amt : unspent credits in blockchain
     * @param block_height: the current height of the blockchain
     */
    void initialize_state(
            std::vector <uint256_t> tx_list,
            uint256_t amt,
            uint32_t block_height);

    uint256_t get_balance();

    /**
     * Direct transaction without executing contract
     *
     * @param pubkey source pubkey
     * @param amt credit received
     *
     * @return bool process result
     */
    bool direct_receive(eevm::Pubkey &pubkey, int amt);

    /**
     * Directly send asset to the target without processing contratc
     *
     * @param pubkey target account
     * @param amt amount of credit to send
     *
     * @return bool the result shows the minus result, once success, TEE account
     * could broad cast the transaction
     */
    bool direct_send(eevm::Pubkey &pubkey, int amt);

    /**
     * Lock credits into the contract instance
     * @param instance_id the instance id of contract
     * @param amt: the amount of credits to be locked
     */
    bool lock_credit(eevm::Instance_id &instance_id, uint32_t amt);

    /**
     * End a contract instacne, the locked credits should be moved into received
     * @param instance_id the instance id of contract
     * @param amt: the amount of credits to be locked
     *
     * @return bool process result
     */
    bool end_contract_instance(eevm::Instance_id &instance_id, uint32_t amt);
};

#endif // EVM_BALANCE_H
