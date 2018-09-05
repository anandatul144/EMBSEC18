#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
 
#include <tss2/tss.h>
#include <tss2/tssutils.h>
#include <tss2/tssresponsecode.h>
#include <tss2/tssprint.h>
#include <tss2/tssmarshal.h>

int verbose = FALSE;		//set TRUE to increase verbosity
int debug = FALSE;			//set TRUE for debug prints
//TSS_SetProperty(NULL, TPM_TRACE_LEVEL, "2");

TPM_HANDLE      sequenceHandle = 0;					//store the handle created for the session
TPMI_ALG_HASH       hashAlg = TPM_ALG_SHA256;		//hash algorithm set to SHA256

/*
Function declarations
*/
TPM_HANDLE hashsequencestart(void);
int hashcalculator(char * inFilename, TPM_HANDLE sequenceHandle);
int sequenceupdate(unsigned char * membuff, size_t len, TPM_HANDLE sequenceHandle);
int sequencecomplete(TPM_HANDLE sequenceHandle);



int main(int argc, char * argv[])
{
	char *inFilename = NULL;
	int i = 1;
	TPM_RC rc = 0;			//TPM response code
	/* If input file name is not present in the command line arguments, exit application */
	if(argc <2)
	{
		printf("Usage : hashextend5 -if <file name>\n");
		exit(1);
	}
	if (strcmp(argv[i],"-if") == 0) {
		if(debug)	{	printf(argv[i],argv[i+1]); printf("\n");	}
		i++;
		if(i<argc)
	    inFilename = argv[i];
		else {
		printf("-if option needs a value\n");
		exit(1);
		}
	}
	else
	{
		printf("Usage : hashextend5 -if <file name>\n");
		exit(1);
	}

	sequenceHandle = hashsequencestart();								//start session and create sequence handle
	if(debug) printf("sequence handle found %x\n",sequenceHandle);	
	if (sequenceHandle)	{												//chain 1KB chunks of data into the TPM
		printf("Computing HASH....please wait...\n");					//to calculate TPM_CC_HASHEXTEND for
		rc = hashcalculator(inFilename, sequenceHandle);				//the created session
	}
	if(rc==0)
		rc = sequencecomplete(sequenceHandle);							//complete hash sequencing, return checksum
    
	printf("end");

	return rc;
}

TPM_HANDLE hashsequencestart(void)
{
	
	TPM_RC          rc = 0;												//return code
	TSS_CONTEXT         *tssContext = NULL;								//context object
	HashSequenceStart_In    in;											//command object
	HashSequenceStart_Out   out;										//output object
	const char          *authPassword = NULL;
	TPMI_SH_AUTH_SESSION        sessionHandle0 = TPM_RH_NULL;
	unsigned int        sessionAttributes0 = 0;
	TPMI_SH_AUTH_SESSION        sessionHandle1 = TPM_RH_NULL;
	unsigned int        sessionAttributes1 = 0;
	TPMI_SH_AUTH_SESSION        sessionHandle2 = TPM_RH_NULL;
	unsigned int        sessionAttributes2 = 0;

	rc = TSS_TPM2B_StringCopy(&in.auth.b, authPassword, sizeof(TPMU_HA));
	in.hashAlg = hashAlg;

	rc = TSS_Create(&tssContext);										//create conext
	if(rc==0)	{
	rc = TSS_Execute(tssContext,										//issue command to TPM to create
				(RESPONSE_PARAMETERS *)&out,							//hashsequence session and return
				(COMMAND_PARAMETERS *)&in,								//sessionhandle
				NULL,
				TPM_CC_HashSequenceStart,
				sessionHandle0, NULL, sessionAttributes0,
				sessionHandle1, NULL, sessionAttributes1,
				sessionHandle2, NULL, sessionAttributes2,
				TPM_RH_NULL, NULL, 0);
	}
	{
	TPM_RC rc1 = TSS_Delete(tssContext);
	if (rc == 0) {
	    rc = rc1;
	}
    }
	
	if (rc == 0) {
	printf("hashsequencestart: handle %08x\n", out.sequenceHandle);
	if (verbose) printf("hashsequencestart: success\n");
    }
    else {
	const char *msg;
	const char *submsg;
	const char *num;
	printf("hashsequencestart: failed, rc %08x\n", rc);
	TSS_ResponseCode_toString(&msg, &submsg, &num, rc);
	printf("%s%s%s\n", msg, submsg, num);
	rc = EXIT_FAILURE;
    }
    return out.sequenceHandle;											//return session object to main
}

int hashcalculator(char * inFilename, TPM_HANDLE sequenceHandle)
{
	size_t len = 0;
	int rc = 0;
	int count = 0;
	size_t MAX_BUFF = 1024;												//buffer size limit
	unsigned char * buff1=NULL;											//buffer to read from file
	unsigned char * membuff=NULL;										//buffer to pass to sequenceupdate()
	
	//membuff = (unsigned char*)malloc(MAX_BUFF);						
		
	FILE *fp = fopen(inFilename,"rb");									//open file in binary read mode
	if(fp == NULL)
	{
		printf("file not found\n");
		return EXIT_FAILURE;
	}
	while(1)
	{	
		buff1 = (unsigned char*)malloc(sizeof(unsigned char*) * MAX_BUFF);
		membuff = (unsigned char*)malloc(sizeof(unsigned char*) * MAX_BUFF);
		if(debug) printf("no seg fault 1\n");
		len = fread(buff1,1,MAX_BUFF,fp);
		if(debug)	printf("length read %u\n",len);
		if(debug) printf("no seg fault 2\n");
		memcpy(membuff,buff1,len);
		if(debug) printf("no seg fault 3\n");
		count++;														//count no of chunks
		rc = sequenceupdate(membuff, len, sequenceHandle);				//pass data and handle to sequenceupdate()
		if(debug) printf("no seg fault 4\n");
		free(buff1);													//free buffers for next read
		if(debug) printf("no seg fault 5\n");
		free(membuff);
		if(debug) printf("no seg fault 6\n");
		if (rc == 0) {
		if(verbose)	printf("sequenceupdate %d of length %d: success\n", count, len);
    	}
    	else return rc;
		
		if (len < MAX_BUFF)												//exit loop when the last chuck is read
		{	break;	}
	}
	fclose(fp);
	
	return rc;
}



int sequenceupdate(unsigned char * membuff, size_t len, TPM_HANDLE sequenceHandle)
{
	TPM_RC			rc = 0;
	TSS_CONTEXT			*tssContext = NULL;
	SequenceUpdate_In 		in;											//sequenceupdate command object
	const char			*sequencePassword = NULL; 
    TPMI_SH_AUTH_SESSION    	sessionHandle0 = TPM_RS_PW;
    unsigned int		sessionAttributes0 = 0;
    TPMI_SH_AUTH_SESSION    	sessionHandle1 = TPM_RH_NULL;
    unsigned int		sessionAttributes1 = 0;
    TPMI_SH_AUTH_SESSION    	sessionHandle2 = TPM_RH_NULL;
    unsigned int		sessionAttributes2 = 0;
	
																		//updating command object
	in.sequenceHandle = sequenceHandle;									//add handler value
	in.buffer.t.size = len;												//lenth of buffer
	memcpy(in.buffer.t.buffer, membuff, len);							//data to be hashed
	
	if (rc == 0) {
	rc = TSS_Create(&tssContext);
    }
	if (rc == 0) {
	rc = TSS_Execute(tssContext,										//issue command to TPM to calculate hash
			 NULL,														//using sequence handler and data stream
			 (COMMAND_PARAMETERS *)&in,									//and update the hash data in the hadler
			 NULL,
			 TPM_CC_SequenceUpdate,
			 sessionHandle0, sequencePassword, sessionAttributes0,
			 sessionHandle1, NULL, sessionAttributes1,
			 sessionHandle2, NULL, sessionAttributes2,
			 TPM_RH_NULL, NULL, 0);
    }
    {
	TPM_RC rc1 = TSS_Delete(tssContext);
	if (rc == 0) {
	    rc = rc1;
	}
    }
	
	if (rc == 0) {
	if (verbose) printf("sequenceupdate: success\n");
    }
    else {
	const char *msg;
	const char *submsg;
	const char *num;
	printf("sequenceupdate: failed, rc %08x\n", rc);
	TSS_ResponseCode_toString(&msg, &submsg, &num, rc);
	printf("%s%s%s\n", msg, submsg, num);
	rc = EXIT_FAILURE;
    }
	
    return rc;
}


int sequencecomplete(TPM_HANDLE sequenceHandle)
{
	TPM_RC			rc = 0;
	TSS_CONTEXT			*tssContext = NULL;
	SequenceComplete_In 	in;											//command object
	SequenceComplete_Out	out;										//output object
    const char			*sequencePassword = NULL; 
    TPMI_SH_AUTH_SESSION    	sessionHandle0 = TPM_RS_PW;
    unsigned int		sessionAttributes0 = 0;
    TPMI_SH_AUTH_SESSION    	sessionHandle1 = TPM_RH_NULL;
    unsigned int		sessionAttributes1 = 0;
    TPMI_SH_AUTH_SESSION    	sessionHandle2 = TPM_RH_NULL;
    unsigned int		sessionAttributes2 = 0;

	size_t length = 0;													//no data for hash calculation

																		//update command object
	in.sequenceHandle = sequenceHandle;									//pass sequence handler
	in.buffer.t.size = length;											//set data length to 0
	in.hierarchy = TPM_RH_NULL;											//set heirarchy
	rc = TSS_Create(&tssContext);										//create context object
    if (rc == 0) {
	rc = TSS_Execute(tssContext,										//issue command to TPM to return
			 (RESPONSE_PARAMETERS *)&out,								//final HASH in object
			 (COMMAND_PARAMETERS *)&in,
			 NULL,
			 TPM_CC_SequenceComplete,
			 sessionHandle0, sequencePassword, sessionAttributes0,
			 sessionHandle1, NULL, sessionAttributes1,
			 sessionHandle2, NULL, sessionAttributes2,
			 TPM_RH_NULL, NULL, 0);
    }
    {
	TPM_RC rc1 = TSS_Delete(tssContext);
	if (rc == 0) {
	    rc = rc1;
	}
    }
    if (rc == 0) {
	TSS_PrintAll("Result", out.result.t.buffer, out.result.t.size);
	//what does out.result.t.buffer print?....it does not work...stupid wrappers
	if (verbose) printf("sequencecomplete: success\n");
    }
    else {
	const char *msg;
	const char *submsg;
	const char *num;
	printf("sequencecomplete: failed, rc %08x\n", rc);
	TSS_ResponseCode_toString(&msg, &submsg, &num, rc);					//print checksum to stdout
	printf("%s%s%s\n", msg, submsg, num);
	rc = EXIT_FAILURE;
    }
    return rc;
}
