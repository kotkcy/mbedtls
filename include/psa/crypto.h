/**
 * \file psa/crypto.h
 * \brief Platform Security Architecture cryptography module
 */

#ifndef PSA_CRYPTO_H
#define PSA_CRYPTO_H

#include "crypto_platform.h"

#include <stddef.h>

#ifdef __DOXYGEN_ONLY__
/** \defgroup platform Implementation-specific definitions
 * @{
 */

/** \brief Key slot number.
 *
 * This type represents key slots. It must be an unsigned integral
 * type.* The choice of type is implementation-dependent.
 * 0 is not a valid key slot number. The meaning of other values is
 * implementation dependent.
 *
 * At any given point in time, each key slot either contains a
 * cryptographic object, or is empty. Key slots are persistent:
 * once set, the cryptographic object remains in the key slot until
 * explicitly destroyed.
 */
typedef _unsigned_integral_type_ psa_key_slot_t;

/**@}*/
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** \defgroup basic Basic definitions
 * @{
 */

/**
 * \brief Function return status.
 *
 * Zero indicates success, anything else indicates an error.
 */
typedef enum {
    /** The action was completed successfully. */
    PSA_SUCCESS = 0,
    /** The requested operation or a parameter is not supported
        by this implementation. */
    PSA_ERROR_NOT_SUPPORTED,
    /** The requested action is denied by a policy. */
    PSA_ERROR_NOT_PERMITTED,
    /** An output buffer is too small. */
    PSA_ERROR_BUFFER_TOO_SMALL,
    /** A slot is occupied, but must be empty to carry out the
        requested action. */
    PSA_ERROR_OCCUPIED_SLOT,
    /** A slot is empty, but must be occupied to carry out the
        requested action. */
    PSA_ERROR_EMPTY_SLOT,
    /** The requested action cannot be performed in the current state. */
    PSA_ERROR_BAD_STATE,
    /** The parameters passed to the function are invalid. */
    PSA_ERROR_INVALID_ARGUMENT,
    /** There is not enough runtime memory. */
    PSA_ERROR_INSUFFICIENT_MEMORY,
    /** There is not enough persistent storage. */
    PSA_ERROR_INSUFFICIENT_STORAGE,
    /** There was a communication failure inside the implementation. */
    PSA_ERROR_COMMUNICATION_FAILURE,
    /** There was a storage failure that may have led to data loss. */
    PSA_ERROR_STORAGE_FAILURE,
    /** A hardware failure was detected. */
    PSA_ERROR_HARDWARE_FAILURE,
    /** A tampering attempt was detected. */
    PSA_ERROR_TAMPERING_DETECTED,
    /** There is not enough entropy to generate random data needed
        for the requested action. */
    PSA_ERROR_INSUFFICIENT_ENTROPY,
    /** The signature, MAC or hash is incorrect. */
    PSA_ERROR_INVALID_SIGNATURE,
    /** The decrypted padding is incorrect. */
    PSA_ERROR_INVALID_PADDING,
    /** An error occurred that does not correspond to any defined
        failure cause. */
    PSA_ERROR_UNKNOWN_ERROR,
} psa_status_t;

/**
 * \brief Library initialization.
 *
 * Applications must call this function before calling any other
 * function in this module.
 *
 * Applications may call this function more than once. Once a call
 * succeeds, subsequent calls are guaranteed to succeed.
 *
 * \return * \c PSA_SUCCESS: success.
 *         * \c PSA_ERROR_INSUFFICIENT_MEMORY
 *         * \c PSA_ERROR_COMMUNICATION_FAILURE
 *         * \c PSA_ERROR_HARDWARE_FAILURE
 *         * \c PSA_ERROR_TAMPERING_DETECTED
 *         * \c PSA_ERROR_INSUFFICIENT_ENTROPY
 */
psa_status_t psa_crypto_init(void);

/**@}*/

/** \defgroup crypto_types Key and algorithm types
 * @{
 */

typedef uint32_t psa_key_type_t;

#define PSA_KEY_TYPE_NONE                       ((psa_key_type_t)0x00000000)
#define PSA_KEY_TYPE_VENDOR_FLAG                ((psa_key_type_t)0x80000000)

#define PSA_KEY_TYPE_CATEGORY_MASK              ((psa_key_type_t)0x7e000000)
#define PSA_KEY_TYPE_RAW_DATA                   ((psa_key_type_t)0x02000000)
#define PSA_KEY_TYPE_CATEGORY_SYMMETRIC         ((psa_key_type_t)0x04000000)
#define PSA_KEY_TYPE_CATEGORY_ASYMMETRIC        ((psa_key_type_t)0x06000000)
#define PSA_KEY_TYPE_PAIR_FLAG                  ((psa_key_type_t)0x01000000)

#define PSA_KEY_TYPE_IS_VENDOR_DEFINED(type) \
    (((type) & PSA_KEY_TYPE_VENDOR_FLAG) != 0)
#define PSA_KEY_TYPE_IS_ASYMMETRIC(type)                                \
    (((type) & PSA_KEY_TYPE_CATEGORY_MASK) == PSA_KEY_TYPE_CATEGORY_ASYMMETRIC)
#define PSA_KEY_TYPE_IS_PUBLIC_KEY(type)                                \
    (((type) & (PSA_KEY_TYPE_CATEGORY_MASK | PSA_KEY_TYPE_PAIR_FLAG) == \
      PSA_KEY_TYPE_CATEGORY_ASYMMETRIC))
#define PSA_KEY_TYPE_IS_KEYPAIR(type)                                   \
    (((type) & (PSA_KEY_TYPE_CATEGORY_MASK | PSA_KEY_TYPE_PAIR_FLAG)) == \
     (PSA_KEY_TYPE_CATEGORY_ASYMMETRIC | PSA_KEY_TYPE_PAIR_FLAG))

typedef uint32_t psa_algorithm_type_t;

#define PSA_ALG_VENDOR_FLAG                     ((psa_algorithm_t)0x80000000)
#define PSA_ALG_CATEGORY_MASK                   ((psa_algorithm_t)0x7f000000)
#define PSA_ALG_CATEGORY_HASH                   ((psa_algorithm_t)0x01000000)
#define PSA_ALG_CATEGORY_MAC                    ((psa_algorithm_t)0x02000000)
#define PSA_ALG_CATEGORY_CIPHER                 ((psa_algorithm_t)0x04000000)
#define PSA_ALG_CATEGORY_AEAD                   ((psa_algorithm_t)0x06000000)
#define PSA_ALG_CATEGORY_SIGN                   ((psa_algorithm_t)0x10000000)
#define PSA_ALG_CATEGORY_ASYMMETRIC_ENCRYPTION  ((psa_algorithm_t)0x12000000)
#define PSA_ALG_CATEGORY_KEY_AGREEMENT          ((psa_algorithm_t)0x22000000)
#define PSA_ALG_CATEGORY_KEY_DERIVATION         ((psa_algorithm_t)0x30000000)

#define PSA_ALG_IS_VENDOR_DEFINED(alg)                                  \
    (((alg) & PSA_ALG_VENDOR_FLAG) != 0)
#define PSA_ALG_IS_HASH(alg)                                            \
    (((alg) & PSA_ALG_CATEGORY_MASK) == PSA_ALG_CATEGORY_HASH)
#define PSA_ALG_IS_MAC(alg)                                             \
    (((alg) & PSA_ALG_CATEGORY_MASK) == PSA_ALG_CATEGORY_MAC)
#define PSA_ALG_IS_CIPHER(alg)                                          \
    (((alg) & PSA_ALG_CATEGORY_MASK) == PSA_ALG_CATEGORY_CIPHER)
#define PSA_ALG_IS_AEAD(alg)                                            \
    (((alg) & PSA_ALG_CATEGORY_MASK) == PSA_ALG_CATEGORY_AEAD)
#define PSA_ALG_IS_SIGN(alg)                                            \
    (((alg) & PSA_ALG_CATEGORY_MASK) == PSA_ALG_CATEGORY_SIGN)
#define PSA_ALG_IS_ASYMMETRIC_ENCRYPTION(alg)                           \
    (((alg) & PSA_ALG_CATEGORY_MASK) == PSA_ALG_CATEGORY_ASYMMETRIC_ENCRYPTION)
#define PSA_ALG_IS_KEY_AGREEMENT(alg)                                   \
    (((alg) & PSA_ALG_CATEGORY_MASK) == PSA_ALG_CATEGORY_KEY_AGREEMENT)
#define PSA_ALG_IS_KEY_DERIVATION(alg)                                  \
    (((alg) & PSA_ALG_CATEGORY_MASK) == PSA_ALG_CATEGORY_KEY_DERIVATION)

/**@}*/

/** \defgroup key_management Key management
 * @{
 */

/**
 * \brief Import a key in binary format.
 *
 * This function supports any output from psa_export_key().
 *
 * \return * \c PSA_SUCCESS: success.
 *         * \c PSA_ERROR_NOT_SUPPORTED
 *         * \c PSA_ERROR_INVALID_ARGUMENT
 *         * \c PSA_ERROR_INSUFFICIENT_MEMORY
 *         * \c PSA_ERROR_COMMUNICATION_FAILURE
 *         * \c PSA_ERROR_HARDWARE_FAILURE
 *         * \c PSA_ERROR_TAMPERING_DETECTED
 */
psa_status_t psa_import_key(psa_key_slot_t key,
                            psa_key_type_t type,
                            const uint8_t *data,
                            size_t data_length);

/**
 * \brief Destroy a key.
 *
 * \return * \c PSA_SUCCESS: success.
 *         * \c PSA_ERROR_EMPTY_SLOT
 *         * \c PSA_ERROR_COMMUNICATION_FAILURE
 *         * \c PSA_ERROR_HARDWARE_FAILURE
 *         * \c PSA_ERROR_TAMPERING_DETECTED
 */
psa_status_t psa_destroy_key(psa_key_slot_t key);

/**
 * \brief Get basic metadata about a key.
 *
 * \return * \c PSA_SUCCESS: success.
 *         * \c PSA_ERROR_EMPTY_SLOT
 *         * \c PSA_ERROR_COMMUNICATION_FAILURE
 *         * \c PSA_ERROR_HARDWARE_FAILURE
 *         * \c PSA_ERROR_TAMPERING_DETECTED
 */
psa_status_t psa_get_key_information(psa_key_slot_t key,
                                     psa_key_type_t *type,
                                     size_t *bits);

/**
 * \brief Export a key in binary format.
 *
 * The output of this function can be passed to psa_import_key() to
 * create an equivalent object.
 *
 * If a key is created with psa_import_key() and then exported with
 * this function, it is not guaranteed that the resulting data is
 * identical: the implementation may choose a different representation
 * of the same key.
 *
 * \return * \c PSA_SUCCESS: success.
 *         * \c PSA_ERROR_EMPTY_SLOT
 *         * \c PSA_ERROR_COMMUNICATION_FAILURE
 *         * \c PSA_ERROR_HARDWARE_FAILURE
 *         * \c PSA_ERROR_TAMPERING_DETECTED
 */
psa_status_t psa_export_key(psa_key_slot_t key,
                            uint8_t *data,
                            size_t data_size,
                            size_t *data_length);

/**@}*/

#ifdef __cplusplus
}
#endif

#include "crypto_extra.h"

#endif /* PSA_CRYPTO_H */
