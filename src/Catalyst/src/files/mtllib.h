#pragma once

namespace Catalyst
{
    namespace files
    {

        typedef struct
        {
            char* map_file;

            bool blendu = true;
            bool blendv = true;

            float boost = 0.0f;

            float mm[2] = { 0.0f, 1.0f };

            unsigned int o[3] = { 0, 0, 0 };
            unsigned int s[3] = { 1, 1, 1 };
            unsigned int t[3] = { 0, 0, 0 };

            unsigned int texres[3] = { 0, 0, 0 };

            bool clamp = true;

            float bm = 1.0f;

            enum scaler_channel
            {
                none, r, g, b, m, l, z
            } imfchan = scaler_channel::none;

            enum reflection_type
            {
                none, sphere, cube_top, cube_front, cube_left, cube_bottom, cube_back, cube_right
            } refltype = reflection_type::none;

        } texture;

        typedef struct 
        {

            char* name;

            float Ka[3] = { 0.0f };
            texture map_Ka;

            float Kd[3] = { 0.0f };
            texture map_Kd;

            float Ks[3] = { 0.0f };
            texture map_Ks;

            float Ns[3] = { 0.0f };
            texture map_Ns;

            float d     = 1.0f;
            float Tr    = 1.0f - d;
            texture map_d;

            float Tf[3] = { 1.0f };

            float Ni = 1.0f;

            int illum = 0;

            texture map_bump;
            texture map_disp;
            texture map_decal;
            texture map_refl;

            float Pr = 0.0f;
            texture map_Pr;

            float Pm = 0.0f;
            texture map_Pm;

            float Ps = 0.0f;
            texture map_Ps;

            float Pc = 0.0f;

            float Pcr = 0.0f;

            float Ke = 0.0f;
            texture map_Ke;

            float aniso = 0.0f;
            float anisor = 0.0f;

        } mtl;
    }
}
