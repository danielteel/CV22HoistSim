// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TcpSocketConnection.h"
#include "TCPControlClient.generated.h"

/**
 *
 */
UCLASS()
class CV22HOISTSIM_API ATCPControlClient : public ATcpSocketConnection {
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	float lastUpdateTime = 0;
	float updateDelta = 0.5;

	float ExtendCommand = 0;
	bool PowerState = false;
	bool JettisonState = false;

	UPROPERTY()
	class ACV22* CV22 = nullptr;

public:
	UFUNCTION()
		void OnConnected(int32 ConnectionId);

	UFUNCTION()
		void OnDisconnected(int32 ConId);

	void UpdateHoistControlPanel();

	UFUNCTION()
		void OnMessageReceived(int32 ConId, TArray<uint8>& Message);

	UFUNCTION(BlueprintCallable)
		void ConnectToGameServer();

	UPROPERTY()
		int32 connectionIdGameServer;
};