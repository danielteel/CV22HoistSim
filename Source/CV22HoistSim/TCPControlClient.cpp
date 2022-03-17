// Fill out your copyright notice in the Description page of Project Settings.

#include "TCPControlClient.h"
#include "HoistControlPanel.h"
#include "HoistComponent.h"
#include "CV22.h"
#include "TailScanner.h"
#include "HeadMountedDisplay/Public/HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"


void ATCPControlClient::ConnectToGameServer() {
	if (isConnected(connectionIdGameServer)) {
		return;
	}
	FTcpSocketDisconnectDelegate disconnectDelegate;
	disconnectDelegate.BindDynamic(this, &ATCPControlClient::OnDisconnected);
	FTcpSocketConnectDelegate connectDelegate;
	connectDelegate.BindDynamic(this, &ATCPControlClient::OnConnected);
	FTcpSocketReceivedMessageDelegate receivedDelegate;
	receivedDelegate.BindDynamic(this, &ATCPControlClient::OnMessageReceived);
	Connect("127.0.0.1", 3002, disconnectDelegate, connectDelegate, receivedDelegate, connectionIdGameServer);
}

void ATCPControlClient::BeginPlay() {
	Super::BeginPlay();
	ConnectToGameServer();
}

void ATCPControlClient::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	if (!isConnected(connectionIdGameServer)) return;

	UpdateHoistControlPanel();
}

void ATCPControlClient::OnConnected(int32 ConId) {
}

void ATCPControlClient::OnDisconnected(int32 ConId) {
	ConnectToGameServer();
}

void ATCPControlClient::UpdateHoistControlPanel() {
	ATailScanner* tailScanner = Cast<ATailScanner>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!tailScanner) return;

	tailScanner->HoistSetUpDown(ExtendCommand);
	tailScanner->HoistSetPower(PowerState);
	tailScanner->HoistSetJettison(JettisonState);
}

void ATCPControlClient::OnMessageReceived(int32 ConId, TArray<uint8>& Message) {
	for (int i = 0; i < Message.Num(); i++) {
		if (Message[i] <= 31) {
			float extendCommand = (float(Message[i]) / 31.0f - 0.5f) * 2.0f;
			if (Message[i] >= 14 && Message[i] <= 16) extendCommand = 0;
			ExtendCommand = extendCommand;
		} else if (Message[i] == 32) {
			PowerState = false;
		} else if (Message[i] == 33) {
			PowerState = true;
		} else if (Message[i] == 34) {
			JettisonState = false;
		} else if (Message[i] == 35) {
			JettisonState = true;
		}
	}
}