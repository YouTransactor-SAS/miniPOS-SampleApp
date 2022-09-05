# YouTransactor uCube Touch miniPOS 

###### Release 1.0.0.9



This repository provides a step by step documentation for YouTransactor's miniPOS SDK, that enables you to integrate our proprietary card terminal(s) to accept credit and debit card payments (incl. VISA, MasterCard, American Express and more).

The miniPOS is a standolone POS solution which has the connectivity capability via WI-FI and 4G/2G. The Payment application runs on the terminal it self. The device integrates 3 processors : Modem processor, Secure processor (SVPP) and WI-FI processor. The modem processor is the applicatif processor and the mobile connectivity provider. The Secure processor manages the communication with the card and the secure operations. The WI-FI processor provides the WI-FI connectivity. 

We give our clients the possibility to develop their payment applicationon the modem, however we provide binaries of the two firmwares WI-FI and secure processor (SVPP). Since the relation between Modem and the two others processor is a Master-Slave relation. So, The MODEM drives this two processors. 
The main function of the SDK is to send RPC commands to the SVPP and the WI-FI in order to drive them. The SDK provides also a payment, update and log APIs.  

The SDK contains several modules:  RPC, Payment, MDM, Network.  

* The RPC module use an SPI interface to send/receive, RPC command/response from secure processor. It provides an implementation of all RPC Commands you will see next how to use that in your application.
* The Network module implement the data exchnage usin mobile data and the WI-FI. The application can use it to call webservices. An example is provided next.
* The MDM module is an implementation of all YouTransactor's TMS services. The TMS server is mainly used to manage the version of firmware and ICC / NFC configurations of card terminal. So the SDK allows you to transparently update of the terminal using our TMS. This module is useless if you decide to use another TMS not the YouTransactor one.
* The payment module implements the EMV transaction processing for contact and contactless. The SDK takes in input a UCubePaymentRequest structure. Is is used to configure the current payment and durring the transaction a callback is returned for every step. At the end of transaction a PaymentContext  structure is returned which contains all necessary data to save the transaction.  
An example of Payment call is provided next.

All this functions are resumed in one Class which is UCubeAPI.

The SDK do not save any transaction or update data.

For more information about [YouTransactor](https://www.youtransactor.com/) products, please refer to our website. Visit [youtransactor.com](https://www.youtransactor.com/)!

## I. General overview

### Introduction:
YouTransactor miniPOS card terminals is a uCube Touch terminal. It is a lightweight and compact payment dongle. 

## I. General overview 

### 1. Introduction

YouTransactor mPOS card terminals are : 
* uCube Touch mPOS
* uCube Touch miniPOS

The uCube Touch exist in two versions, first one needs the smartphone to communication with the server, the secod one is a standalone version. Which has the connectevity and the payment application runs directly on the device.

This github is specific to the miniPOS version. For more information about mPOS version please refers to these github repo
* [Android SDK](https://github.com/YouTransactor-SAS/mPOS-SDK-Android/)
* [IOS SDK](https://github.com/YouTransactor-SAS/mPOS-SDK-IOS/)


### 3. uCube Touch

The uCube Touch is a lightweight and compact payment dongle. It is a point of sale, that accepts contactless and contact payment cards.

<p align="center">
  <img width="250" height="250" src="https://user-images.githubusercontent.com/59020462/77367842-437df080-6d5b-11ea-8e3a-423c3bc6b96b.png">
</p>

### 1. The Management System

The management system can be administered by YouTransactor and offers the following services:

- Management of the uCube fleet

- Deployment of software updates

- Deployment of payment parameters

- Other services

The MDM module of SDK implements all our management system services and the UCubeAPI provides methods to call this implementation.

### 2. Terminal management

#### 2.1 Initial configuration
To be functional, in the scope of PCI PTS requirement, and SRED key shall be loaded securely in the device. This key is loaded locally by YouTransactor tools. The initial SALT is injected in the same way.

#### 2.2 Switching On/Off
The uCube lights up by pressing the "ON / OFF" button for three seconds. Once the device is on, the user can initiate the payment process.  
The uCube switches off either by pressing the "ON / OFF" button or after X* minutes of inactivity (* X = OFF timeout).

#### 2.3 Update
During the life of the terminal, the firmware can be updated (to get bug fix, evolutions..), the contact and contactless configuration also can be updated. The Terminal's documentation describe how these updates can be done and which RPC to use to do that.

## II. Technical Overview

### 1. General Architecture

#### 1.1 Hardware Architecture                                       
uCube Touch miniPOS integrates a Quectel modem (BG95-M3) and a St-microelectronics secure processor (STM32L4).  
The modem is running the SDK software including the application demo “appmodem” while the secure processor is running the SVPP software that is controlling display, keyboard and has interface with EMV contact and contactless modules.  
  
![architecture](https://user-images.githubusercontent.com/55090899/188469960-79d45167-f3ea-48c0-bcb4-9c20f2a21a36.png)
 
#### 1.2 Application Architecture
This diagrams describes the general software architecture. Only the uCubeAPI methods and the RPC commands are public and you can call them.  

![sdk_architecture](https://user-images.githubusercontent.com/55090899/188472350-01bdadc3-d42a-4b21-afc8-1494bf8134a3.png)

The demo application we provide display a menu as a first screen, user will be able to select one of the options example 'Payment', so the application will call the UCube_api_pay with a uCubePaymentRequest intance as input.   
This starts a transaction and enable the contact and the contactless readers. Depends on the activated reader one of the transaction flows will be called.  
This sequence diagram below shows the interaction between different soft modules before the menu is showen and when user press Payment option.
  
![sequence](https://user-images.githubusercontent.com/59020462/159264182-f60ffd83-e5df-40d5-8a20-067e90bc9d75.png)

### 2. Transaction Flow : Contact

![contact_flow](https://user-images.githubusercontent.com/59020462/159264254-7ae827f1-df93-4463-ac2b-0e6deee01c48.jpeg)

### 3. Transaction Flow : Contactless

![contactless](https://user-images.githubusercontent.com/59020462/159264392-375f7822-2956-4810-aace-772eb9df45e5.jpeg)

### 4. UCubeAPI

The APIs provided by UCubeAPI are:

```c
// start a payment
 /*
  * use this api when you need to start a transaction
  * the SDK implement the contact and the contactless transaction flow
  * @param UCubePaymentRequest_t : object with all needed data in input of the transaction
  * and all callbacks that the SDK will call at a specific moment of the transaction.
  * e.g. onProgress() callback, AuthorisationTask callback,...etc.
  * The paymentContext object is created at the begin of transaction, his input variable are set 
  * from the begin of the transaction, they can be updated during that and all output variable 
  * are set during and at the end of the transaction. 
  * */ 
error_t  UCube_api_pay(UCubePaymentRequest_t* uCubePaymentRequest);

// connect to network using cellular connectivity
error_t  UCube_api_connect(network_handler_t * network_handler);

// send a HTTP request
error_t  UCube_api_send_http_request(socket_handler_t * socket_handler);

// send  RPC 
uint8_t UCube_api_send_Data(svpp_com_transaction_param_u  rpc_param,
         uint32_t rpc_id);
```

#### 4.1 Payment  

The SDK implements the payment state machine, both contact and contactless. You configure your transaction using the uCubePaymentRequest_t object by specifing a value for each attribut, for instance, the transaction amount, currency, type, ...etc.  

#### pay API

Create a Payment Request variable :

```c
  UCubePaymentRequest_t UCubePaymentRequest_ct;
```

Call the UCube_api_pay and pass a pointer to the uCubePaymentRequest:

```c
  UCube_api_pay(&UCubePaymentRequest_ct);
```  

The input parameter of Pay API is the uCubePaymentRequest. This class contains all input variables of a payment. At the begin of the transaction, the SDK create a new instance of PaymentContext and save into it all the input values. Here is an example of preparing a uCubePaymentRequest object, all variables are explained in the PaymentContext section :

```c
error_t preparePaymentRequest(UCubePaymentRequest_t * UCubePaymentRequest) {

    error_t err = ERRORNO;
    
    Language preferredLanguageList[1];
    
    Currency CURRENCY_EUR;
    
    CardReaderType readerList[2] = {ICC,NFC};

    Date date;
    
    uint16_t authorizationPlainTags[]={TAG_4F_APPLICATION_IDENTIFIER,TAG_50_APPLICATION_LABEL };
    
    uint16_t authorizationSecuredTags[]={
            TAG_SECURE_5A_APPLICATION_PRIMARY_ACCOUNT_NUMBER,
            TAG_SECURE_57_TRACK_2_EQUIVALENT_DATA,
            TAG_SECURE_56_TRACK_1_DATA,
            TAG_SECURE_5F20_CARDHOLDER_NAME,
            TAG_SECURE_5F24_APPLICATION_EXPIRATION_DATE,
            TAG_SECURE_5F30_PAYMENT_CODE,
            TAG_SECURE_9F0B_CARDHOLDER_NAME_EXTENDED,
            TAG_SECURE_9F6B_TRACK_2_DATA
    };
  
    uint16_t finalizationPlainTags[]={
              TAG_9F06_APPLICATION_IDENTIFIER__TERMINAL,
              TAG_9F10_ISSUER_APPLICATION_DATA,
              TAG_9F1A_TERMINAL_COUNTRY_CODE    
    };
    
    uint16_t finalizationSecuredTags[]={
            TAG_SECURE_5F24_APPLICATION_EXPIRATION_DATE,
            TAG_SECURE_5F30_PAYMENT_CODE        
    };

    UCubePaymentRequest->authorizationPlainTags_len =  sizeof(authorizationPlainTags)/sizeof(uint16_t);
    
    UCubePaymentRequest->authorizationSecuredTags_len = sizeof(authorizationSecuredTags)/sizeof(uint16_t);
    
    UCubePaymentRequest->finalizationPlainTags_len =  sizeof(finalizationPlainTags)/sizeof(uint16_t);
    
    UCubePaymentRequest->finalizationSecuredTags_len = sizeof(finalizationSecuredTags)/sizeof(uint16_t);

    CURRENCY_EUR.currency_code[0] = (uint8_t)((SVPP_COM_CUREENCCY_EURO & 0xFF00) >> 8);
    
    CURRENCY_EUR.currency_code[1] = (uint8_t)(SVPP_COM_CUREENCCY_EURO & 0x00FF);
    
    CURRENCY_EUR.currency_exp = 2;

    preferredLanguageList[0].language[0] = 'e';
    
    preferredLanguageList[0].language[1] = 'n';

    UCubePaymentRequest->amount =1;
    
    UCubePaymentRequest->currency = CURRENCY_EUR;
    
    UCubePaymentRequest->transactionType = PURCHASE;
    
    UCubePaymentRequest->readerList = readerList;
    
    UCubePaymentRequest->authorizationTask = authorizationTask;
    
    UCubePaymentRequest->riskManagementTask = riskManagementTask;
    
    UCubePaymentRequest->applicationSelectionTask = applicationSelectionTask;
    
    UCubePaymentRequest->preferredLanguageList = preferredLanguageList;

    UCubePaymentRequest->onProgress = onProgress ;
    
    UCubePaymentRequest->onFinish = onFinish;
    

     //Add optional variables
     
    UCubePaymentRequest->forceOnlinePin = FALSE;
    
    err = get_date(&date);
    
    if(!err)
    {
       UCubePaymentRequest->transactionDate= date;
       
    }
    UCubePaymentRequest->forceAuthorisation = FALSE;
    
    UCubePaymentRequest->cardWaitTimeout = 10000;
    
    UCubePaymentRequest->systemFailureInfo2 = FALSE,
    
    UCubePaymentRequest->forceDebug = FALSE;
    
    UCubePaymentRequest->skipCardRemoval = TRUE;
    
    UCubePaymentRequest->authorizationPlainTags = authorizationPlainTags;
    
    UCubePaymentRequest->authorizationSecuredTags =authorizationSecuredTags;
    
    UCubePaymentRequest->finalizationPlainTags = finalizationPlainTags;
    
    UCubePaymentRequest->finalizationSecuredTags =finalizationSecuredTags;
    
    UCubePaymentRequest->onlinePinBlockFormat = PIN_BLOCK_ISO9564_FORMAT_0;
    
    UCube_api_pay(UCubePaymentRequest);
    
    return err;
}

```

During the payment the  OnProgress() callback is called. The payment Context and State can be checked:

```c
void onProgress(UCubePaymentContext_t* context,PaymentState* state){

    PRINTF("********Payment State: %s \r\n", getStateName(*state));
    //TODO Put here your code
    
}
```

At the end of payment the Payment Context is returned and the callback onFinish is called:

```c
void onFinish(UCubePaymentContext_t* context){
   
    PRINTF("********Payment Status: %s \r\n", getStatusName(context->paymentStatus));
    //TODO Put here your code
  
}
```
- You can use the example provided in the customer_app (payment.c)

#### PaymentContext

The PaymentContext is the object that evoluates for each step of the payment and is returned at the end.

```c

//  payment context
typedef struct
{
  
    /***************************************** input *******************************************/
    /*
     * Amount of the transaction, as defined in [EMV-2].
     * If the Currency exponent is set to 2, and Amount is 100,
     * the transaction price will be 1,00
     * the amount could maximun have 12 digit
     * default value is -1 means no amount will be passed byt the application
     * and the terminal will request the amount at the begin of the transaction
     * */
    int amount;
    
    /*
     * Indicates the currency of the transaction
     * The Currency class has 3 attributes :
     *  String label;
     *  int code; // according to ISO 4217
     *  int exponent;
     * */
    Currency currency;
    
    /*
     * Indicates the type of financial transaction, represented by the first two digits of the
     * ISO 8583:1987 Processing Code. The actual values to be used for the Transaction Type data
     * element are defined by the payment system. The supported Transaction type are the following one:
     *  * PURCHASE
     *  * WITHDRAWAL
     *  * REFUND
     *  * PURCHASE_CASHBACK
     *  * MANUAL_CASH
     *  * INQUIRY
     * NOTE: the support of these Transaction Type depends on the configuration.
     * */
    TransactionType transactionType;
    
    /*
     * Local date & time that the transaction was authorised
     * */
    Date transactionDate;
    
    /*
     * Timeout for "waiting for any interfaces"
     * NOTE: The timeout is limited to 0xFF (255 seconds)
     * */
    int cardWaitTimeout;
    
    /*
     * 1 to 6 languages stored in order of preference, each represented by 2 alphabetical
     * characters according to ISO 639
     * */
    uint8_t * preferredLanguageList;
    
    /*
     * only for the contactless transaction, if true, force the execution of this step
     * NFC_SIMPLIFIED_ONLINE_PIN to get the online pin block
     * */
    int forceOnlinePIN;
    
    /*
     * For contactless, it enable the force online at the start nfc transaction
     * for the contact, it enable the Merchant force online : byte 4 bit 4 of the TVR
     * */
    int forceAuthorization;
    
    /*
     * Requested PIN block format:
     *  0x00 – ISO 9564 format 0
     *  0x01 – ISO 9564 format 1
     *  0x03 – ISO 9564 format 3
     *  0x04 – ISO 9564 format 4
     *  Default is 0
     * */
    int onlinePinBlockFormat;
    
    /*
     * The different interfaces to be activated
     * */
    CardReaderType * readerList;
    
    /*
     * if true, the SDK will retrieve the 0xF4 and 0xCC tags at the end of the transaction
     * the 0xF4 and 0xCC tags contain part of SVPP Level 2 logs
     * */
    uint8_t forceDebug;
    
    /*
     * if true, the SDK will retrieve the 0xF4 and 0xCC tags at the end of the transaction
     * this flag could be enabled by the application during one of the tasks for instance the
     * authorisationTask if the backend decide to decline the transaction.
     * */
    uint8_t getSystemFailureInfoL2;
    
    /*
     * the list of tags need to be retrieved before calling the authorisationTask
     * */
    uint16_t * authorizationPlainTags;
    uint16_t * authorizationSecuredTags;
    
    /*
     * the list of tags need to be retrieved before ending the transaction
     * */
    uint16_t * finalizationPlainTags;
    uint16_t * finalizationSecuredTags;

    /*icc input */
    /*
     * if true, the SDK will skip the SMC_REMOVE_CARD and does not wait for card to be removed
     * and go through the transaction result
     * */
    uint8_t skipCardRemoval;
    

    /* output common */
    
    PaymentStatus paymentStatus;// The payment status see below possible values  
    
    uint8_t * uCubeInfos; // If the GET_INFO was called, it will contains the terminal firmware version
    
    uint8_t sredKsn[10];  // Key serial number (SMID) includes current transaction key counter for the  data encryption 
    
    uint8_t pinKsn[10];   // MANDATORY if an Online PIN has been entered.Null if not.
    
    uint8_t onlinePinBlock[8]; // The returned pin block formatted with the onlinePinBlockFormat specified in the input 
    
    uint8_t activatedReader;   // The activated interface
    
    uint16_t * finalizationPlainTagsValues; // A table of key value that contains all requested finalization plain tags
    
    uint16_t * authorizationPlainTagsValues;// A table of key value that contains all requested authorisation plain tags
    
    uint8_t * finalizationGetPlainTagsResponse; // the whole terminal's response of the getPlainTags command to be checked by the backend
    
    uint16_t * finalizationSecuredTagsValues;// the whole terminal's response of the getSecuredTags command to be checked & parsed by the backend
    
    uint16_t * authorizationGetPlainTagsResponse;//the whole terminal's response of the getPlainTags command to be checked by the backend
    
    uint16_t * authorizationSecuredTagsValues; // the whole terminal's response of the getSecuredTags command to be checked & parsed by the backend
    
    /*
     * This variable need to be set after calling the authorisation backend
     * It contains the host response
     * */
    uint16_t * authorizationResponse;
    
    /***************************************** output *******************************************/
    
    /* output icc */
    
    /*
     * The object that describe the selected application
     * the EMVApplicationDescriptor has these attributes :
     *  private byte[] aid;
     *  private String label;
     *  private int priority;
     *  private int issuerCodeIndex;
     *  private byte selectionOptions;
     *  private boolean blocked;
     *  private byte[] languagePreference;
     *  private byte[] fci;
     *  private byte[] productId;
     * */
    EMVApplicationDescriptor selectedApplication;
    
    /*
     * The terminal verification result
     * */
    uint8_t tvr[5];
    
    /*
     * The whole response of the TransactionFinalisation command with header and footer
     * */
    uint8_t transactionFinalisationData[2];
    
    /*
     * The whole response of the TransactionInitialization command with header and footer
     * */
    uint8_t transactionInitData[2];
    uint8_t transactionProcessData[2];
    
    /* output nfc */
    /*
     * The OUTCOME is composed of 2 bytes:
     *  Byte 0:
     *  - 0x30: Receipt
     *  - 0x31: Receipt, Signature required
     *  - 0x32: No Receipt
     *  - 0x34: Online PIN Request
     *  Byte 1:
     *  - 0x36: APPROVED
     *  - 0x3E: ONLINE_REQUEST
     *  - 0x31: TRY_ANOTHER_INTERFACE
     *  - 0x3A: TRANSACTION_CANCELLED
     *  - 0x3F: END_APPLICATION
     *  - 0x37: DECLINED
     *  - 0x38: FAILED
     * Example: "0x3036"
     *
     * */
    uint8_t nfcOutcome[2];
    
    /*
     * When the byte 0 of the outcome equals to 0x31,
     * this flag will be enabled and the application nned to
     * request the cardholder signature
     * */
    uint8_t signatureRequired;
    
    /* output for debug */
    uint8_t tagCC;  // svpp logs level 2 Tag CC
    
    uint8_t tagF4;  // svpp logs level 2 Tag F4
    
    uint8_t tagF5;  // svpp logs level 2 Tag F5

}UCubePaymentContext_t;

```
#### PaymentState

You will receive the onProgress() callback for each new state. 

This is the whole liste of payement states :

```c
typedef enum{
	/* COMMON STATES*/
	
	//start
	ENTER_SECURE_SESSION,
	
	KSN_AVAILABLE,
	
	DISPLAY_START_WAIT_CARD,
	
	START_TRANSACTION,
	
	CARD_READ_END,

	AUTHORIZATION,

	//end
	GET_FINALIZATION_SECURED_TAGS,
	
	GET_FINALIZATION_PLAIN_TAGS,
	
	GET_CC_L2_LOG,
	
	GET_F4_L2_LOG,
	
	GET_F5_L2_LOG,
	
	END_EXIT_SECURE_SESSION,
	
	/* SMC STATES*/
	START_ICC,
	
	SMC_BUILD_CANDIDATE_LIST,
	
	SMC_SELECT_APPLICATION,
	
	SMC_USER_SELECT_APPLICATION,
	
	SMC_INIT_TRANSACTION,
	
	SMC_RISK_MANAGEMENT,
	
	SMC_PROCESS_TRANSACTION,
	
	SMC_GET_AUTHORIZATION_SECURED_TAGS,
	
	SMC_GET_AUTHORIZATION_PLAIN_TAGS,
	
	SMC_FINALIZE_TRANSACTION,
	
	SMC_REMOVE_CARD,
	
	/* NFC STATES*/
	START_NFC,
	
	NFC_GET_AUTHORIZATION_SECURED_TAGS,
	
	NFC_GET_AUTHORIZATION_PLAIN_TAGS,
	
	NFC_SIMPLIFIED_ONLINE_PIN,
	
	NFC_COMPLETE_TRANSACTION,
	
}PaymentState;

```
#### EMV Payment state machine

![payment_state_machine](https://user-images.githubusercontent.com/59020462/159264897-8a768f0e-e96c-4602-a06a-50bd49ccaea9.png)

The EMV payment state machine is sequence of executing commands and tasks. Bellow you will see the different tasks used at transaction

#### Tasks
Durring the payment process the payment state machine will be interrupted to execute some tasks that you implement.

#### ApplicationSelectionTask

```c
void applicationSelectionTask(EMVApplicationDescriptor *
		appList, uint8_t *size)
{
	   // TODO execute your application select algorithm here
}
```

#### RiskManagementTask

```c
uint8_t riskManagementTask(void){

	//TODO call your application risk management process
	
	//return response
}
```
#### AuthorizationTask

```c
uint8_t authorizationTask (void){

	//TODO call you backend here
	
       // at the end return the response
}
```

#### PaymentStatus

```c
typedef enum{
    APPROVED, // Transaction has been approved by terminal
    
    DECLINED, // Transaction has been declined by terminal

    /* Cancelled Status cases:
    
        1/ GPO not read yet and user app call payment.cancel()
        
        2/ one of commands returns -32 or -28 status
        
        3/ NFC_Outcome[1] = 0x3A Transaction_cancelled
        
    */
    CANCELLED,

    CARD_WAIT_FAILED, //Transaction has been failed because customer does not present a card and startNFCTransaction fail
    
    UNSUPPORTED_CARD, //Transaction has been failed: Error returned by terminal, at contact transaction, when no application match between card and terminal's configuration
    
    REFUSED_CARD, // TRANSACTION PROCESS RETURNS EMV NOT ACCEPT

    NFC_OUTCOME_TRY_OTHER_INTERFACE, // Transaction has been failed: Error returned by terminal, at contactless transaction
    
    NFC_OUTCOME_END_APPLICATION, // Transaction has been failed: Error returned by terminal, at contactless transaction
    
    NFC_OUTCOME_FAILED, // Transaction has been failed: Error returned by terminal, at contactless transaction

    ERROR, // Transaction has been failed : when one of the tasks or commands has been fail
    
    ERROR_SHUTTING_DOWN, //Transaction has been failed : when command fails with SHUTTING_DOWN error during the transaction
    
    ERROR_DISCONNECT, //Transaction has been failed : when there is a disconnect during the transaction
    
    ERROR_WRONG_ACTIVATED_READER, // Transaction has been failed : when terminal return wrong value in the tag DF70 at startNFCTransaction
    
    ERROR_MISSING_REQUIRED_CRYPTOGRAM, // Transaction has been failed :when the value of the tag 9f27 is wrong
    
    ERROR_WRONG_CRYPTOGRAM_VALUE, // Transaction has been failed : when in the response of the transaction process command the tag 9F27 is missing
    
    ERROR_WRONG_NFC_OUTCOME, // Transaction has been failed : when terminal returns wrong values in the nfc outcome byte array

}PaymentStatus;

```
#### 4.2 Connectivity

#### Connect to cellular network

Create a network_handler_t variable:

```c
network_handler_t  network_handler_ct;
```

Initialize the network APN and set the prefered_network in the network handler to the cellular network. Then, call the UCube_api_connect.

```c
#define QL_DEF_APN_CT  "orange" 

error_t init_network(network_handler_t * network_handler) {

    error_t err = ERRORNO;
    
    strlcpy(network_handler->cellular_conf.apn, QL_DEF_APN_CT, QAPI_DSS_CALL_INFO_APN_MAX_LEN);
    
    network_handler->prefered_network = PREFERED_NETWORK_CELLULAR;
    
    UCube_api_connect(network_handler);
    
    return err;
}

```
#### Send HTTP request

Create a socket_handler_t variable :

```c
    socket_handler_t socket_handler_ct;
```

Define the request url and type (GET/POST) as below:

```c
    #define HTTP_REQUEST_URL  "https://webhook.site/5680215f-0091-45ab-b7b6-e2a9651df81e"
```

Initialize the HTTP request using in your socket_handler_t variable.Then, call the UCube_api_send_http_request with a pointer to your socket_handler. 

```c
error_t init_http_request(socket_handler_t * socket_handler) {

    error_t err = ERRORNO;

    // define url of request
    
    memset(socket_handler->request_url, 0, sizeof(socket_handler->request_url));
    
    sprintf((char*)socket_handler->request_url, HTTP_REQUEST_URL);
    
    socket_handler->url_length = strlen((const char *)socket_handler->request_url);
    
    socket_handler->request_type = HTTPS_CLIENT_GET_E ;
    
    socket_handler->requestResult_cb = requestResult_ct_cb;
    
    UCube_api_send_http_request(socket_handler);

    return err;
}
```

The request callback requestResult_ct_cb defined in init_http_request() is called at the end of http request.
Example is provided below: it displays the response code received from the server.

```c
void requestResult_ct_cb (https_request_handler_t request_handler){

    PRINTF("**Request result %d\r\n", request_handler.resp_Code);
    // TODO Put your code here
}
```

### 5. RPC Commands

```c
/************************************ System & Drivers ************************************/

/* (RPC ID : 5001)
 * The command gets a set of device informations. 
 * */
    svpp_com_ident_getinfo_t ident_getinfo;

/* (RPC ID : 5003)
 * The command set device information. Note, this command can be called once the device is in the field.  
 * */
    svpp_com_ident_setinfofield_t ident_setinfofield;

/* (RPC ID : 5002)
 * The command sets several device information. Note, this command cannot be called once the device is in the field. 
 * */
    svpp_com_ident_setinfoprod_t ident_setinfoprod;

/* RPC ID : 5020
 * This function waits for card insertion on a set of slots, in parallel. 
 * In case a smart card is inserted, the card is powered on automatically (cold reset).
 * Note: This command can be cancelled by the cardholder (Cancel key pressed by the cardholder) 
 * */   
    svpp_com_card_drv_cardwaitinsertion_t card_drv_cardwaitinsertion;

/* (RPC ID : 5021)
 * This command sends a power off to a smart card previously inserted. 
 * */
    svpp_com_card_drv_cardwaitremoval_t card_drv_cardwaitremoval;

/* (RPC ID : 5023)
 * This command sends a power off to a smart card previously inserted. 
 * */   
    svpp_com_card_drv_cardpoweroff_t card_drv_cardpoweroff;

/* (RPC ID : 5022) 
 * This command sends an APDU to a smart card previously inserted and powered, respectfully to [EMV-1]. 
   * */
    svpp_com_card_drv_cardsendapdu_t card_drv_cardsendapdu;

/* (RPC ID : 5024)   
 *This function returns the magnetic stripe data. See Magstripe Payment Workflow section 7.1.  
 * */    
    svpp_com_card_drv_magstriperead_t card_drv_magstriperead;

/* (RPC ID : 5040)
 * This command is used to display a message on the screen without user Key Input. 
 * Only the OK, CANCEL and ABORT keys are monitored, and returned back. 
 * The text is given in command parameters. The command will answer only when:
    • The timeout is reached or
    • One of the configured “abortkey” is pressed.
 * The command defines a full display. A clear screen is performed before each command execution 
 * in order to erase remaining previous text.  
 * */
    svpp_com_hmi_drv_displaymessagewithoutki_t hmi_drv_displaymessagewithoutki;

/* (RPC ID : 5043)
 *  This command is used to display a message on the terminal screen, and getting back the  
 *  plaintext numeric keys entered by the user. Only secured text messages can be displayed.  
 *  These messages are secured because they have been previously verified by YouTransactor  
 *  that sign them after verification (with the K_KTCC).  
 *  Prompt texts are built up in the following way, whereby %s indicates where the input is added and the ‘|’    
 *  starts the input format. The input format string is shown on the display at the place indicated by %s.   
 *  The ‘_’   characters are placeholder for the expected input. Optionally, a default value can be added after  
 *  the   input format and a second ‘|’ will starts the default value. The default value will be displayed  
 *  instead of   “_”. A clear screen is performed before and after each command execution in order to erase  
 *  remaining previous   text.Section 6.1.12     
 * */    
    svpp_com_hmi_drv_displaymessagewithki_t hmi_drv_displaymessagewithki;

/*  (RPC ID : 5045)
 *  This command is an evolution of HMI_DRV.DisplayMessageWithKI (5043) in order to fulfil a Manual PAN entry  
 *  scenario in which multiple prompts are necessary to gather all necessary data and format them before any  
 *  SRED   encryption.  
 *  
 *  Additional and optional TLVs in this command payload will allow a fine tuned format of the final expected   
 *  output. Then the full Manual PAN entry scenario could be done like follow:  

    • STATE_MACHINE.EnterSecuredSession (5101)  
    • HMI_DRV.DisplayMessageWithKICustom (5045)  
    • HMI_DRV.DisplayMessageWithKICustom (5045)  
    • PAY.GetSecuredTagValue (5025)  
    • STATE_MACHINE.ExitSecuredSession (5102)   
 *  Section 6.1.13  
 * */
    svpp_com_hmi_drv_displaymessagewithkicustom_t hmi_drv_displaymessagewithkicustom;  

/* (RPC ID : 5042)
 * This command is used to display a list box. The keys ABORT and OK are evaluated during this command. 
 * The list is build up in the order the text strings are given. 
 * A clear screen is performed before and after each command execution in order to erase remaining previous text
 * */
    svpp_com_hmi_drv_displaylistboxwithoutki_t hmi_drv_displaylistboxwithoutki;

/* (RPC ID : 0302)
 * This command is used to set time in dongle. Time is composed of day, month, year, hour, minute, second
 * */
    svpp_com_term_rtcset_t term_rtcset;
 
/* (RPC ID : 0301)
 * This command is used to set time in dongle. Time is composed of day, month, year, hour, minute, second 
 * */   
    svpp_com_term_rtcget_t term_rtcget;
 
/* (RPC ID : 0202)
 * This command is used to cancel all asynchronous process.
 * */
    svpp_com_term_cancelall_t term_cancelall;


/************************************ Security Kernel ************************************/
  
/*  (RPC ID : 5101)
 * If this command is called during the:
 *   • READY state: the product switches from READY state to SECURED state. 
 *   This command will internally increment the DUKPT key counter and generates session keys for a new transaction.
 *   All internal data or states which may be left from a previous transaction are cleared during the call.
 *   • PERSO state: the product switches from PERSO to READY. 
 *   It can no more switching back to PERSO state any more after this action.
 *   • PRE PERSO state: the product switches from PRE PERSO to PERSO. 
 *   It can no more switching back to PRE PERSO state any more after this action.
 * */  
    svpp_com_state_machine_entersecuredsession_t  state_machine_entersecuredsession;

/* (RPC ID : 5110)
 * This command is used to receive information about stored keys on the terminal   
 * */  
    svpp_com_keymngt_getkeyinfo_t keymngt_getkeyinfo;

/* (RPC ID : 5120)
 * This command is used to export an RSA public key to the LCI tool, in order to generate a certification  
 * signing request in a secured room during production stage. The only exportable public key is the K_KEK_PUB  
 * The certificate is injected with the KEYMNGT.ImportPublicKeyCert.   
 * */    
    svpp_com_keymngt_exportpublickey_t keymngt_exportpublickey;

/* (RPC ID : 5121)
 * This command is used by the LCI tool to import an RSA public key certificate. The certificate is verified by  
 * the terminal according to the K_CA_ROOT_PUB, or the K_CA_PROD_PUB depending of the certificate to be loaded.  
 * By this way the certification chain is verified.    
 * If a certificate is loaded (K_CA_ROOT_PUB, K_CA_PSP_PUB ,K_HSM_PUB, K_CA_PROD_PUB, K_KEK_PUB), it is impossible  
 * to update it by a new one. Deletion or replacement of certificates is impossible (except after reloading the  
 * full terminal software) Section 6.2.5   
 * */   
    svpp_com_keymngt_importpublickeycert_t keymngt_importpublickeycert;
  
/* (RPC ID : 5122)
 * This command is used in PERSO state to import K_CA_PSP_SUB. The certificate is verified by the terminal according to the  
 * K_CA_PSP.
 * If K_CA_PSP_SUB certificate is already loaded, it is impossible to update it by a new one. Deletion or replacement of this  
 * certificate is impossible (except when reloading the full terminal software)  
 * */       
    svpp_com_keymngt_importpublickeypersocert_t keymngt_importpublickeypersocert;

/* (RPC ID : 5130)
 * This command is used to export all the necessary information to the RKI server 
 * in order to perform the further DUKPT initial key injection. 
 * */
    svpp_com_keymngt_installforloadkey_t keymngt_installforloadkey;
 
/* (RPC ID : 5131)
 * This command is used to inject DUKPT initial key. The key to load is nested in an [ANSI TR-31]key exchange block protected with  
 * a key block protection key. The key block protection key is transmitted encrypted with the terminal K_KEK_PUB key and signed by  
 * the remote host with the K_HSM_PRIV key    
 * */
    svpp_com_keymngt_loadkey_t keymngt_loadkey;
 
/*  (RPC ID : 5160)
 * This command initializes a download sequence. Section 6.2.9
 * */
    svpp_com_download_installforload_t download_installforload;
   
/* (RPC ID : 5161)
 * This command sends the Data File to load. This command is sent in several Blocks. 
 * Once the last block is received by the SVPP, the code installed once the signature has been verified.
 * */
    svpp_com_download_load_t download_load;
    
/* (RPC ID : 5170)
 * This command processes an online PIN entry and return back the encrypted PIN block using the DUKPT PIN session key  
 * As the PIN value can be wrong, this command can be called several times during a transaction (the beginning of a transaction is  
 * identified with STATE_MACHINE.EnterSecuredSession). 
 * The KSN counter is incremented at the first call after the beginning of the transaction. It remains then unchanged if called   
 * again  within the same transaction.    
 * */   
    svpp_com_pin_onlinepin_t pin_onlinepin;
    
/* (RPC ID : 5171)
 * This command processes an online PIN entry and return back the encrypted PIN block using the DUKPT PIN session key.
 * */
    svpp_com_pin_simplifiedonlinepin_t pin_simplifiedonlinepin;
  
/************************************ Payment kernel ************************************/


/* (RPC ID : 5501)
 * Before executing any transaction, a set of banking parameters must be initialized. 
 * These banking parameters provided to this function are only the one handled by EMVL2 
 * (AID list for application selection public keys to perform cards offline authentication, 
 * and Certificate Revocation List: CRL). 
 * Note that the parameters provided by the bank, but corresponding to the local payment scheme 
 * should be handled by the calling application.
 * */
    svpp_com_pay_bankparametersdownload_t pay_bankparametersdownload;
    
/* (RPC ID : 5503)
 * This command retrieve previously set bank parameters. 
 * */
    svpp_com_pay_bankparametersget_t pay_bankparametersget;

/* (RPC ID : 5510)
 * The execution of this command presumes that a card is already inserted in the device, and powered on.
 * This command process the matching between the EMV applications supported by the terminal, and the one 
 * supported by the card (with card supporting PSE or list of AID) with the Build candidate process list 
 * defined in [EMV-1], section 12.3. It builds the candidate list and returns the mutually supported applications
 * list with, for each application, the AID, the preferred name ( if the issuer table index is supported, 
 * see [ ISO/IEC 8859] ) or the label, the application priority indicator and the issuer table index. 
 * This command provides back all the necessary information to enable the calling application to choose the
 * appropriated AID according to its local scheme specificities. If more than one application can be chosen, 
 * the calling application will ask for cardholder choice through the DisplayListBox function. 
 * Once the AID selected, the PAY.TransactionInitialization command is called with this AID in input to continue
 * the transaction sequence.
 * */
    svpp_com_pay_buildcandidatelist_t pay_buildcandidatelist;

/* (RPC ID : 5511)
 * The execution of this command presumes that a card is already inserted in the device, and powered on, and the
 * BuildCandidateList has previously been called.
 * This command:
 *   • Initializes the transaction, with an amount, currency, etc.
 *   • Performs the FINAL SELECTION, with the Final selection according to [EMV-1], section 12.4
 *   • Performs the GET PROCESSING OPTIONS followed by the subsequent READ RECORDS, according to [EMV-3], 
 *     section 10.1 & 10.2.
 *   • Select the cardholder language. In case a card language matches with a language provided by 
 *   the calling application, it is automatically selected. In case of multiple choices, 
 *   a DisplayListBox appears on the screen to ask the user to select his preferred language.
 * This command provides back all the necessary information to enable the calling application to 
 * perform an acquire risk management (black list, b check, etc...). Once the result of the 
 * risk management is known, the calling application sends the TransactionProcess to continue 
 * the transaction sequence.
 * */    
    svpp_com_pay_transactioninitialization_t pay_transactioninitialization;  
    
 /* (RPC ID : 5512)
 * The execution of this command presumes that a card is already inserted in the device,
 * and powered on, and the PAY.TransactionInitialization has previously been called.
 * This command:
 *   • Performs the PROCESSING RESTRICTIONS according to [EMV-3], section 10.4
 *   • Performs the PIN ENTRY according to [PCI PTS] security requirements, according to the cardholder 
 *     selected language during PAY.TransactionInitialization, and according to [EMV-3], section 10.5
 *   • Performs the OFFLINE DATA AUTHENTICATION (SDA, DDA, SDA) according to [EMV-3], section 10.3
 *   • Performs the TERMINAL RISK MANAGEMENT according to [EMV-3], section 10.6 (Floor Limits, 
 *      Random Transaction Selection, Velocity Checking)
 *   • Performs the TERMINAL ACTION ANALYSIS (first generate AC) according to [EMV-3], section 10.7
 * This command provides back all the necessary information to enable the calling application to perform 
 * an authorisation to the acquire
 * */   
    svpp_com_pay_transactionprocess_t pay_transactionprocess;
    
 /* (RPC ID : 5513)
 * The execution of this command presumes that a card is already inserted in the device, and powered on,
 * and the TransactionProcess has previously been called.
 * This command:
 *   • Performs the ISSUER DATA AUTHENTICATION according to [EMV-3], section 10.9 (Online processing / external
 *   authenticate)
 *   • Performs the ISSUER SCRIPT PRCESSING according to [EMV-3], section 10.10. It
 *       ◦ Applies SCRIPT 71
 *       ◦ Performs the SECOND GENERATE AC
 *       ◦ Applies SCRIPT 72
 * This command provides back all the necessary information to finalise a transaction. 
 * Note, at the end of this command, even if the transaction is accepted, the calling application
 * can later refuse it (for instance if the cardholder receipt printing failed).
 * */   
    svpp_com_pay_transactionfinalisation_t pay_transactionfinalisation;
     
/* (RPC ID : 5025)
 * This function returns the value of one given tag. 
 * The tag can be an EMV tag or a proprietary tag. 
 * NOTE: If only one TAG is provided in input, the output will ONLY contain the value (and not the format TLV)
 * SRED tags: 56 – 57 - 5A – 5F20 – 5F24 – 5F30 – 9F0B – 9F6B
 * */
    svpp_com_pay_getsecuredtagvalue_t pay_getsecuredtagvalue;
    
/* (RPC ID : 5026)
 * This function returns the value of one given tag. 
 * The tag can be an EMV tag or a proprietary tag. 
 * */
    svpp_com_pay_getplaintagvalue_t pay_getplaintagvalue;
    
/* (RPC ID : 5530)
 * This function starts a NFC Transaction.
 * */
    svpp_com_pay_nfcstarttransaction_t pay_nfcstarttransaction; 
 
/* (RPC ID : 5531)
 * This function completes a NFC Transaction (if necessary)
 * */   
    svpp_com_pay_nfccompletetransaction_t pay_nfccompletetransaction;
```

For each RPC there is a corresponding parameter defined in svpp_com_gen_data.h. Initialize the parameter's fields with the required values.
The example below shows how to initialize and send a get info RPC. The required parameter fields is tag.
The tag is initialized with the value of svpp version wich is : 0xD1 defined in SVPP_COM_TAG_INFO_SVPP_VERSION.
The response received in the parameter field service_ctx.rpc_ctx.param.

```c

void init_rpc(void)
{
    error_t err;
    
    uint8_t svpp_version[4];
    
    svpp_com_transaction_param_u param_ct;
    
    uint8_t tag =  0xD1;
    
    param_ct.ident_getinfo.tx_msg.pData = &tag;
    
    param_ct.ident_getinfo.tx_msg.len_data= sizeof(tag);
    
    err= UCube_api_send_Data(param_ct,(uint32_t)SVPP_COM_IDENT_GETINFO_CMD_NB);
    
    param_ct = *payment_ctx.rpc_ctx.param;
    
    if(err == ERRORNO ) {
    
        if((memcmp(param_ct.ident_getinfo.rx_msg.pData,&tag,sizeof(tag)))==0) {
	
            if(param_ct.ident_getinfo.rx_msg.pData[1] == sizeof(svpp_version)) {
	    
                memcpy(svpp_version, &param_ct.ident_getinfo.rx_msg.pData[2],  sizeof(svpp_version));
                        
                PRINTF("********** SVPP VERSION :");
                
                PRINTF_HEX_BUFFER(svpp_version,4);
                
                PRINTF("\r\n :");
            }
        }
    }
}
   
```

All this commands are described in the terminal documentation.

If the device is in secured state, the input / output data may be protected by a specific security level. The terminal documentation describes how input data and output data are protected for every command in each different security state.   
 
Note : In the secure session there is a sequence number managed by the SDK and incremented at every RPC call, If you need to know what is the current sequence number you cann get it using getCurrentSequenceNumber API.
- In the case of output, the SDK create e RPCMessage to store response.

Switch case of protection level, the parse of response will be different :
- In the case of none, it will be the same parse as Ready state, only commandId, status & data contain values.
- In the case of signed, commandId, status, data & data_mac contain values.
- In the case of signed and ciphered, commandId, status, data, data_mac & data_ciphered contain values.
Note that no MAC if the data is null.

## III. Task Creation

### 1.Introduction:
To create a task use the os_itf interface. Follow the sample code provided in customer_app.

### 2. Task Creation

Create and init task function to be called from appmodem.c. Create a queue and assign it to the your task. The task priority, stack size, name, and routine need to be passed to os_itf_task_create() function.

```c
/**
 * @brief    intialize customer app
 * create customer task and queue
 * @return  error
 */
error_t customer_app_init(void)
{
    error_t err = EFAIL;
    
    /* create queue for customer task*/
    err = os_itf_queue_create(&customer_app_ctx.queue,
                                QUEUE_CUSTOMER_APP_NAME,
                                OS_ITF_MAX_QUEUE_MSG_SIZE,
                                CUSTOMER_APP_MAX_MESSAGE_IN_QUEUE);

    if (err == ERRORNO){
    
        customer_app_ctx.task.stacksize = TASK_CUSTOMER_APP_STACK;
        customer_app_ctx.task.priority = TASK_CUSTOMER_APP_PRIORITY;
        customer_app_ctx.task.function = customer_app_routine;
        memcpy(customer_app_ctx.task.threadName, \
                TASK_CUSTOMER_APP_THREAD_NAME, \
                strlen(TASK_CUSTOMER_APP_THREAD_NAME));
        /* create  customer task*/
        err = os_itf_task_create(&customer_app_ctx.task);

        if (err != ERRORNO){
            PRINTF_VERBOSE("ERROR: customer_app_init, os_itf_task_create %d", err);
        }
    }
    
    else{
        PRINTF_VERBOSE("ERROR: customer_app_init, os_itf_queue_create %d", err);
    }

    return err;
   
}
```

### 3.Task Routine

Create the task routine and enter at as input to the function os_itf_task_create() as shown above.

Example:

```c
/**
 * @brief    main of customer app
 * @return  NULL
 */
TASK_FUNCTION_RETURN_TYPE customer_app_routine(TASK_FUNCTION_ARGS)
{

    error_t err = ERRORNO;
    
    for(;;){
    
      //TODO
       Put your code here
       
    }
    TASK_FUNCTION_RETURN_INSTRUCTION
    
}
```

### 4.Queue 
#### 4.1 send message to queue
To send a message to queue create the message with type "OS_ITF_QUEUE_API_ARG" and use the function os_itf_queue_send to send it.  

Example for sending a message to customer task:

```c          
            
 os_itf_queue_msg_t msg_from_main_to_customer_task;
 
 OS_ITF_QUEUE_API_ARG msg_to_queue = (OS_ITF_QUEUE_API_ARG)&msg_from_main_to_customer_task;
 
 msg_from_main_to_customer_task.type = MESSAGE_RECV;
 
 msg_from_main_to_customer_task.size = 1;
 
 msg_from_main_to_customer_task.payload.pData = &startApp;
 
 os_itf_queue_send(&customer_app_ctx.queue, msg_to_queue); 
                   
```
#### 4.2receive message in queue

To receive a message in queue, use the function  os_itf_queue_rcv() and indicate in parameter the queue and a pointer to the message to receive.

Example from customer task:

```c

/**
 * @brief    main of customer app
 * @return  NULL
 */
TASK_FUNCTION_RETURN_TYPE customer_app_routine(TASK_FUNCTION_ARGS)
{

    OS_ITF_QUEUE_API_ARG msg_from_queue;
    
    int status = -1;
    
    for(;;){
    
        /*receive message in queue */
        status = os_itf_queue_rcv(&customer_app_ctx.queue, &msg_from_queue);

        //TODO
        
        Put your code here
    }
    
    TASK_FUNCTION_RETURN_INSTRUCTION
}

```


![Cptr_logoYT](https://user-images.githubusercontent.com/59020462/71242500-663cdb00-230e-11ea-9a07-3ee5240c6a68.jpeg)
