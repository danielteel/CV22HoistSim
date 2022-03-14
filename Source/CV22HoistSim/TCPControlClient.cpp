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

	if (!CV22) {
		CV22 = Cast<ACV22>(UGameplayStatics::GetActorOfClass(GetWorld(), ACV22::StaticClass()));
	}
	if (!CV22) return;

	UHoistComponent* hoistComponent = Cast<UHoistComponent>(CV22->GetComponentByClass(UHoistComponent::StaticClass()));
	if (!hoistComponent) return;

	float currentTime = GetWorld()->TimeSeconds;
	if (fabs(lastUpdateTime - currentTime) > updateDelta) {
		FHitResult hitResult;
		FCollisionQueryParams params;
		params.AddIgnoredActor(CV22);
		bool result = GetWorld()->LineTraceSingleByChannel(hitResult, CV22->GetActorLocation(), CV22->GetActorLocation() + (FVector::DownVector * 20000), ECollisionChannel::ECC_PhysicsBody, params);
		uint8 radalt = 255;
		if (result) {
			hitResult.Distance = hitResult.Distance - 205;
			hitResult.Distance *= 0.0328084f;
			hitResult.Distance /= 2;
			if (hitResult.Distance < 0) {
				radalt = 0;
			} else if (hitResult.Distance > 254) {
				radalt = 255;
			} else {
				radalt = hitResult.Distance;
			}
		}
		lastUpdateTime = currentTime;
		uint8 hoistOutLength = hoistComponent->GetHoistOutLength() * 0.0328084f;
		SendData(connectionIdGameServer, { radalt, hoistOutLength });
	}
}

void ATCPControlClient::OnConnected(int32 ConId) {
}

void ATCPControlClient::OnDisconnected(int32 ConId) {
	ConnectToGameServer();
}

void ATCPControlClient::UpdateHoistControlPanel() {
	if (!CV22) {
		CV22 = Cast<ACV22>(UGameplayStatics::GetActorOfClass(GetWorld(), ACV22::StaticClass()));
	}
	if (!CV22) return;

	UHoistControlPanel* hoistControlPanel = Cast<UHoistControlPanel>(CV22->GetComponentByClass(UHoistControlPanel::StaticClass()));
	if (!hoistControlPanel) return;


	hoistControlPanel->SetExtendCommand(ExtendCommand);
	hoistControlPanel->SetPowerState(PowerState);
	if (JettisonState) {
		hoistControlPanel->Jettison();
	}
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
	UpdateHoistControlPanel();
}