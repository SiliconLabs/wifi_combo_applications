/*
 * Copyright 2012 Xinxin Fan <fan@openecc.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * - Redistributions of source code must retain the above copyright 
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright   
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __ECDSA_H__
#define __ECDSA_H__


/**
 * Modular multiplication over GF(p)
 */
void modpmul(uint32 *a, uint32 *b, uint32 *amulb, uint32 curve_id);

/**
 * Modular inversion over GF(p)
 */
void modpinv(uint32 *a, uint32 *b, uint32 curve_id);
uint16 test1(void);

 

/**

Creates an ECDSA signature of a message digest.
The outputs are the r and s components of the signature.

@param[in] privateKey The private key. This is an unsigned char buffer of size
                      <tt>SECT163K1_PRIVATE_KEY_SIZE</tt>.
@param[in] msgDigest  The hash of the message to be signed. This is an unsigned
                      char buffer of size <tt>AES_MMO_HASH_SIZE</tt>.
@param[in] GetRandomData Pointer to a function to get random data for
                         generating ephemeral keys.
@param[in] yieldLevel The yield level determines how often the user defined yield
                      function will be called. This is a number from <tt>0</tt> to <tt>10</tt>.
                      <tt>0</tt>  will never yield.
                      <tt>1</tt>  will  yield the most often.
                      <tt>10</tt> will yield the least often.
@param[in] YieldFunc  Pointer to a function to allow user defined yielding.
@param[out] r The r component of the signature. This is an unsigned char buffer
              of size <tt>SECT163K1_POINT_ORDER_SIZE</tt>.
@param[out] s The s component of the signature. This is an unsigned char buffer
              of size <tt>SECT163K1_POINT_ORDER_SIZE</tt>.

@retval MCE_ERR_NULL_PRIVATE_KEY    <tt>privateKey</tt> is <tt>NULL</tt>.
@retval MCE_ERR_NULL_OUTPUT_BUF     <tt>msgDigest</tt>, <tt>r</tt> or
                                    <tt>s</tt> are <tt>NULL</tt>.
@retval MCE_ERR_NULL_FUNC_PTR       <tt>GetRandomData</tt> is <tt>NULL</tt> or
                                    <tt>YieldFunc</tt> is <tt>NULL</tt> and
                                    <tt>YieldLevel</tt> is not <tt>0</tt>.
@retval MCE_ERR_BAD_INPUT           <tt>YieldLevel</tt> is greater than <tt>10</tt>.
@retval MCE_SUCCESS                 Success.
*/


 
uint32 ZSE_ECDSASign(uint8 *privateKey,uint8 *msgDigest,uint32 (*randum_pointer)(uint32 rlen, uint8 *rnd),uint8 *r,uint8 *s , uint16 (*yield) (void),uint32 yieldLevel);


/**

Verifies an ECDSA signature created using a private signing key by using
the associated public key, the digest and the signature components.

@param[in] publicKey The public key. This is an unsigned char buffer of size
                     <tt>SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE</tt>.
@param[in] msgDigest The hash of the message to be verified.  This is an
                     unsigned char buffer of size <tt>AES_MMO_HASH_SIZE</tt>.
@param[in] r         The r component of the signature. This is an unsigned char
                     buffer of size <tt>SECT163K1_POINT_ORDER_SIZE</tt>.
@param[in] s         The s component of the signature. This is an unsigned char
                     buffer of size <tt>SECT163K1_POINT_ORDER_SIZE</tt>.
@param[in] yieldLevel The yield level determines how often the user defined yield
                      function will be called. This is a number from <tt>0</tt> to <tt>10</tt>.
                      <tt>0</tt>  will never yield.
                      <tt>1</tt>  will  yield the most often.
                      <tt>10</tt> will yield the least often.
@param[in] YieldFunc  Pointer to a function to allow user defined yielding.
                      <tt>YieldFunc</tt> may be <tt>NULL</tt> if <tt>yieldLevel</tt> is <tt>0</tt>.

@retval MCE_ERR_FAIL_VERIFY        The signature verification failed.
@retval MCE_ERR_NULL_PUBLIC_KEY    <tt>publicKey</tt> is <tt>NULL</tt>.
@retval MCE_ERR_NULL_INPUT_BUF     <tt>msgDigest</tt>, <tt>r</tt> or
                                   <tt>s</tt> are <tt>NULL</tt>.
@retval MCE_ERR_NULL_FUNC_PTR      <tt>YieldFunc</tt> is <tt>NULL</tt> and
                                   <tt>YieldLevel</tt> is not <tt>0</tt>.
@retval MCE_ERR_BAD_INPUT          <tt>YieldLevel</tt> is greater than <tt>10</tt>.
@retval MCE_SUCCESS                Success.
*/

uint32 ZSE_ECDSAVerify(unsigned char *publicKey, unsigned char *msgDigest, unsigned char *r,unsigned char *s , uint16 (*yield) (void),uint32 yieldLevel);

#endif /* __ECDSA_H__ */  
