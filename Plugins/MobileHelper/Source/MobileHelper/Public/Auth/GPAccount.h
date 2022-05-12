// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#endif

#include "GPAccount.generated.h"

UCLASS(BlueprintType)
class MOBILEHELPER_API UGPAccount : public UObject
{
	GENERATED_BODY()

public:
	UGPAccount() = default;
	virtual ~UGPAccount();

#if PLATFORM_ANDROID
	void Init(jobject AccountObject);
#endif

	/**
	* Get the account's display name (requires permission)
	*
	* @return - Account's display name
	*/
	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|Account")
	FString AccountName() const;

	/**
	* Get the account's id (requires permission)
	*
	* @return - Account's id
	*/
	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|Account")
	FString AccountId() const;

	/**
	* Get the account's access token (requires permission)
	*
	* @return - Account's access token
	*/
	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|Account")
	FString AccessToken() const;

	/**
	* Get the account's email (requires permission)
	*
	* @return - Account's email
	*/
	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|Account")
	FString Email() const;

private:
#if PLATFORM_ANDROID
	jobject Account;
	jmethodID GetDisplayNameMethod;
	jmethodID GetAccountIdMethod;
	jmethodID GetEmailMethod;
	jmethodID GetIdTokenMethod;
#endif
};
