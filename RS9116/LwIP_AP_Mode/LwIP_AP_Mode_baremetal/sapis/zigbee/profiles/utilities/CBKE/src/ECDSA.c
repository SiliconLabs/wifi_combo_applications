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

#include "OpenECC.h"
#include "ECDSA.h"
#include "GF2n.h"
#include "ECCK.h"
#include "GFp.h"
#include "AES128.h"
#include "AESMMO.h"

/**************************************************************************************
 modpmul(.): Modular multiplication over GF(p)
 **************************************************************************************/
void modpmul(uint32 *a, uint32 *b, uint32 *amulb, uint32 curve_id)
{
	uint32 i, j, Num;
	uint32 c[2*N-1], p[N+1], q[N+1];
	uint64 T1, T2, T3;

	switch(curve_id){
	case sect163k1: Num = 6;
		break;
	case sect233k1: Num = 8;
		break;
	default: return;
	}

	//c = a * b
	T1 = (uint64)(*a) * (*b);
	*c = (uint32)T1;
	T1 = T1 >> 32;

	for(i = 1; i < 2*(Num - 1); i++)
	{
		T3 = 0;
		for(j = (i < Num ? 0 : (i - Num + 1)); j <= (i < Num ? i : (Num - 1)); j++)
		{
			T2 = (uint64)(*(a + j)) * (*(b + (i - j)));
			T1 += (uint32)T2;
			T3 += (T2 >> 32);
		}
		*(c + i) = (uint32)T1;
		T1 = (T1 >> 32) + T3;
	}

	*(c + 2*(Num - 1)) = (uint32)T1 + (*(a + (Num - 1))) * (*(b + (Num - 1)));

	//Barrett Modular Reduction 
	T1 = (uint64)(*(c + Num - 1)) * (*(eccu[sect163k1]));
	T1 = T1 >> 32;

	for(i = Num; i < Num * 2; i++)
	{
		T3 = 0;
		for(j = Num - 1; j <= (i < (2*Num - 1) ? i : 2*(Num - 1)); j++)
		{
			T2 = (uint64)(*(c + j)) * (*(eccu[sect163k1] + (i - j)));
			T1 += (uint32)T2;
			T3 += (T2 >> 32);
		}
		T1 = (T1 >> 32) + T3;
	}

	for(i = 0; i < Num - 1; i++)
	{
		T3 = 0;
		for(j = i + Num; j <= 2*(Num - 1); j++)
		{
			T2 = (uint64)(*(c + j)) * (*(eccu[sect163k1] + (Num * 2 + i - j)));
			T1 += (uint32)T2;
			T3 += (T2 >> 32);
		}
		*(p + i) = (uint32)T1;
		T1 = (T1 >> 32) + T3;
	}

	*(p + (Num - 1)) = (uint32)T1;

	//q = p * eccn
	T1 = (uint64)(*p) * (*(eccn[sect163k1]));
	*q = (uint32)T1;
	T1 = T1 >> 32;

	for(i = 1; i < Num - 1; i++)
	{
		T3 = 0;
		for(j = (i < (Num/2) ? 0 : (i - Num/2 + 1)); j <= i; j++)
		{
			T2 = (uint64)(*(p + j)) * (*(eccn[sect163k1] + (i - j)));
			T1 += (uint32)T2;
			T3 += (T2 >> 32);
		}
		*(q + i) = (uint32)T1;
		T1 = (T1 >> 32) + T3;
	}

	T2 = (uint64)(*p) * (*(eccn[sect163k1] + (Num - 1)));
	T1 += (uint32)T2;
	T3 = T2 >> 32;
	for(j = Num/2; j <= Num - 1; j++)
	{
		T2 = (uint64)(*(p + j)) * (*(eccn[sect163k1] + (Num - 1 - j)));
		T1 += (uint32)T2;
		T3 += (T2 >> 32);
	}
	*(q + Num - 1) = (uint32)T1;
	T1 = (T1 >> 32) + T3;

	T2 = (uint64)*(p + 1) * (*(eccn[sect163k1] + (Num - 1)));
	T1 += (uint32)T2;
	for(j = Num/2 + 1; j <= Num - 1; j++)
	{
		T2 = (uint64)(*(p + j)) * (*(eccn[sect163k1] + (Num - j)));
		T1 += (uint32)T2;
	}
	*(q + Num) = (uint32)T1;

	for(i = Num - 1; i != 0; i--)  
		*(p + i) = *(eccn[sect163k1] + i);
	*p = *(eccn[sect163k1]);
	*(p + Num) = 0;

	if(sub(c, q, q, Num + 1) == 1)                 
	{
		for(i = Num - 1; i != 0; i--)  
			*(amulb + i) = *(q + i);
        *amulb = *q;
		return;
	}

	while(compare(q, p, Num + 1) != -1)  
		sub(q, p, q, Num + 1);
	for(i = Num - 1; i != 0; i--)  
		*(amulb + i) = *(q + i);
	*amulb = *q;
}

/**************************************************************************************
 modpinv(.): Modular inversion over GF(p)
 **************************************************************************************/
 void modpinv(uint32 *a, uint32 *b, uint32 curve_id)
 {
	 uint32 g1[N] = {0x1};
	 uint32 g2[N] = {0x0};
	 uint32 u[N];
	 uint32 v[N];
	 uint32 p[N] = {0x0};
	 uint32 i, Num, uv, t;

	 switch(curve_id){
	 case sect163k1:
		 Num = 6;
		 XTOY(u, a, 6);
		 XTOY(v, eccn[0], 6);
		 uv = (IFN1_163(u)) && (IFN1_163(v));
		 break;
	 case sect233k1:
		 Num = 8;
		 XTOY(u, a, 8);
		 XTOY(v, eccn[1], 8);
		 uv = (IFN1_233(u)) && (IFN1_233(v));
		 break;
	 default: return;
	 }

	 sub(p, eccn[curve_id], p, Num);

	 while(uv)
	 {
		 while((u[0] & 0x1) == 0) 
		 {
			 for(i = 0; i < Num - 1; i++)
				 *(u + i) = (*(u + i) >> 1) | (*(u + i + 1) << 31);
			 *(u + Num - 1) = *(u + Num - 1) >> 1;

			 if((g1[0] & 0x1) != 0)
				 add(g1, eccn[curve_id], g1, curve_id);

			 for(i = 0; i < Num - 1; i++)
				 *(g1 + i) = (*(g1 + i) >> 1) | (*(g1 + i + 1) << 31);
			 *(g1 + Num - 1) = *(g1 + Num - 1) >> 1;
		 }

		 while((v[0] & 0x1) == 0) 
		 {	
			 for(i = 0; i < Num - 1; i++)
				 *(v + i) = (*(v + i) >> 1) | (*(v + i + 1) << 31);
			 *(v + Num - 1) = *(v + Num - 1) >> 1;

			 if((g2[0] & 0x1) != 0)
				 add(g2, eccn[curve_id], g2, curve_id);
	
			 for(i = 0; i < Num - 1; i++)
				 *(g2 + i) = (*(g2 + i) >> 1) | (*(g2 + i + 1) << 31);
			 *(g2 + Num - 1) = *(g2 + Num - 1) >> 1;
		 }

		 if(compare(u, v, Num) != -1)
		 {
			 sub(u, v, u, Num);
			 if(sub(g1, g2, g1, Num) == 1)
				 sub(g1, p, g1, Num); 
		 }
		 else
		 {
			 sub(v, u, v, Num);
			 if(sub(g2, g1, g2, Num) == 1)
				 sub(g2, p, g2, Num);
		 }

		 switch(curve_id){
	     case sect163k1:
			 uv = (IFN1_163(u)) && (IFN1_163(v));
		     break;
	     case sect233k1:
		     uv = (IFN1_233(u)) && (IFN1_233(v));
		     break;
	     default: return;
		 }
	 }

	 switch(curve_id){
	 case sect163k1:
		 t = IF1_163(u);
		 break;
	 case sect233k1:
		 t = IF1_233(u);
		 break;
	 default: return;
	 }

	 if(t)
	 {
		 for(i = Num - 1; i != 0; i--)
			 *(b + i) = *(g1 + i);
		 *b = *g1;
	 }
	 else 
	 {
		 for(i = Num - 1; i != 0; i--)
			 *(b + i) = *(g2 + i);
		 *b = *g2;
	 } 
 }
 

uint32 ZSE_ECDSASign(uint8 *privateKey,uint8 *msgDigest,uint32 (*randum_pointer)(uint32 rlen, uint8 *rnd),uint8 *r,uint8 *s ,uint16 (*yield) (void),uint32 yieldLevel)
 
{ 
	
 
	uint32 k[N], e[N] ;
	uint32 i, Num;
	ec_point_aff R;
	uint8 rnd1[29];
	 
	int curve_id=sect163k1;
	uint32 *p,*q ;
	p=(uint32*)r;
	q=(uint32*)s;
	Num = 6;
	 

if(privateKey == NULL)
 return MCE_ERR_NULL_PRIVATE_KEY;
if(msgDigest == NULL)
return MCE_ERR_NULL_OUTPUT_BUF;
if(randum_pointer == NULL)
return MCE_ERR_NULL_FUNC_PTR;
	 
	 
	if(randum_pointer(232,rnd1) == 0)
	printf("error in random num generation");

	*(k + 5) = (uint32)(*(rnd1 + 20)) & 0x7;
  


	for(i = 0; i < Num - 1; i++)
	{
		*(k + i) = (uint32)(*(rnd1 + 4*i + 3));
		*(k + i) = (*(k + i) << 8) | (uint32)(*(rnd1 + 4*i + 2));
		*(k + i) = (*(k + i) << 8) | (uint32)(*(rnd1 + 4*i + 1));
		*(k + i) = (*(k + i) << 8) | (uint32)(*(rnd1 + 4*i));
	}
	
	if(compare(k, eccn[curve_id], Num) != -1)
		sub(k, eccn[curve_id], k, Num);

	TNAF5_fixed_scalarmul(k, &R, curve_id);
	//XTOY((uint32*)r, R.x, Num);
	//while(compare((uint32*)r, eccn[curve_id], Num) != -1)
	//	sub((uint32*)r, eccn[curve_id], (uint32*)r, Num);

	 
	XTOY(p, R.x, Num);
	while(compare(p, eccn[curve_id], Num) != -1)
		sub(p, eccn[curve_id], p, Num);

	for(i = 0; i < 4; i++)
	{
		*(e + i) = ((uint32)*(msgDigest+4*i) << 24) | ((uint32)*(msgDigest+4*i+1) << 16) | ((uint32)*(msgDigest+4*i+2) << 8) | ((uint32)*(msgDigest+4*i+3));
		*(e + i + 4) = 0x0;
	}

 
	modpmul(p,(uint32*)privateKey , q, curve_id);
	modpadd(q, e, q, curve_id);
	modpinv(k, e, curve_id);
	modpmul(q, e, q, curve_id);
	
 

	/*
	modpmul((uint32*)r,(uint32*) privateKey, (uint32*)s, curve_id);
	modpadd((uint32*)s, e, (uint32*)s, curve_id);
	modpinv(k, e, curve_id);
	modpmul((uint32*)s, e, (uint32*)s, curve_id);
	 
	printf("The signature are:\n");
		printf("  r = ");for(i = 0; i < 22; i++){printf("%x ",r[i]);}printf("\n");
		printf("  s = ");for(i = 0; i < 22; i++){printf("%x ",s[i]);}printf("\n"); 
	*/
	 

	if(IF0_163(q)) return 0;

	return MCE_SUCCESS;
}

/***************************************************************************************************
  ECDSA signature verification
  **************************************************************************************************/


uint32 ZSE_ECDSAVerify(unsigned char *Qx, unsigned char *msgDigest, unsigned char *r,unsigned char *s , uint16 (*yield) (void),uint32 yieldLevel)
{
	 
	uint32 si[8], u[8], e[8] ;
	uint32 *p,*q ;
	
    int curve_id=sect163k1;
	uint32 i, Num, v;
	ec_point_aff R, S, Q;
	ec_point_pro Rp;
	Num = 6;
 	 
	p=(uint32*)r;
	q=(uint32*)s;
	if(Qx == NULL)
	return MCE_ERR_NULL_PUBLIC_KEY;
	if(msgDigest == NULL)
	return MCE_ERR_NULL_INPUT_BUF;



	if(compare(p, eccn[curve_id], Num) != -1 || compare(q, eccn[curve_id], Num) != -1)
		return 0;
	
	for(i = 0; i < 4; i++)
	{
		*(e + i) = ((uint32)*(msgDigest+4*i) << 24) | ((uint32)*(msgDigest+4*i+1) << 16) | ((uint32)*(msgDigest+4*i+2) << 8) | ((uint32)*(msgDigest+4*i+3));
		*(e + i + 4) = 0x0;
	}
	modpinv(q, si, curve_id);
	modpmul(e, si, e, curve_id);
	modpmul(p, si, u, curve_id);
	TNAF5_fixed_scalarmul(e, &R, curve_id);
	if(point_decompression((uint32*)Qx, &Q, curve_id) == 0)
		return 0;
	TNAF5_random_scalarmul(u, &Q, &S, curve_id);
	affine_to_project(&R, &Rp, curve_id);
	mixed_addition(&Rp, &S, &Rp, curve_id);
	project_to_affine(&Rp, &R, curve_id);

	switch(curve_id){
	case sect163k1:
		v = (IF0_163(R.x)) && (IF0_163(R.y));
		break;
	case sect233k1: 
		v = (IF0_233(R.x)) && (IF0_233(R.y));
		break;
	default: return 0;
	}

	if(v) return 0;
	
	XTOY(e, R.x, Num);
	while(compare(e, eccn[curve_id], Num) != -1)
		sub(e, eccn[curve_id], e, Num);

	if(compare(e, p, Num) == 0) 
		return MCE_SUCCESS;
	else return MCE_ERR_FAIL_VERIFY;
}









                   
                    
                    


uint16 test1(void)
{

	return 1;


}
