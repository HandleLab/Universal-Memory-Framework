#include "pch.h"
#include "features.hpp"


PlayerStruct* Features::GetBestTarget(Vector3* origin, float fov) {
    PlayerStruct* botTarget{ nullptr };

    // sécurité check des ptr
    if (g_Mem->GetEntityList() == 0 || g_Mem->GetLocalPlayer() == 0) return nullptr;

    // get entityCount (+ cast type)
    unsigned int count = g_Mem->Read<unsigned int>(g_Mem->GetEntityCount());
    if (count == 0) return nullptr;

    // get LocalPlayer (besoin de cast)
    PlayerStruct* player = reinterpret_cast<PlayerStruct*>(g_Mem->GetLocalPlayer());
    if (!player || player->dead) return nullptr;

    Vector3 forwPlayer = Math::CalculateForwardVector(player->viewAngle.yaw, player->viewAngle.pitch);

    // boucle sur les bots
    for (unsigned int i = 1; i < count; i++) {
        
        PlayerStruct* bot = g_Mem->Read<PlayerStruct*>(g_Mem->GetEntityList() + (i * 4));

        if (bot != nullptr) {
            if (bot == player || bot->dead) continue;

            
            if (bot->team != 1 && bot->team == player->team) continue;

            // Calcul du score
            Vector3 dirToBot = Math::deltaPos(player->pos, bot->pos);
            float magniDelta = Math::magnitude(&dirToBot);
            Vector3 normDirToBot = Math::normalisation(&dirToBot, magniDelta);
            float score = Math::dotProduct(&forwPlayer, &normDirToBot);

            if (score > 0.5f && score < 1.0f) {
                botTarget = bot;
                return botTarget;
            }
        }
    }
    return botTarget;
}