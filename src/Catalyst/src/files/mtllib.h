#pragma once

namespace Catalyst
{
    namespace files
    {

        struct texture
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

            enum class scaler_channel
            {
                none, r, g, b, m, l, z
            } imfchan = scaler_channel::none;

            enum class reflection_type
            {
                none, sphere, cube_top, cube_front, cube_left, cube_bottom, cube_back, cube_right
            } refltype = reflection_type::none;

        };

        struct mtl
        {

            std::string name;

            //Ambient
            float Ka[3] = { 0.0f };
            texture map_Ka;

            //Diffuse
            float Kd[3] = { 0.0f };
            texture map_Kd;

            //Specular
            float Ks[3] = { 0.0f };
            texture map_Ks;

            //Specular Exponent
            float Ns[3] = { 0.0f };
            texture map_Ns;

            //Transparency
            float d     = 1.0f;
            texture map_d;
            //Inverse
            float Tr    = 1.0f - d;

            //Transmission Filter Color
            float Tf[3] = { 1.0f };

            //Optical Density / Index of Refraction
            float Ni = 1.0f;

            //Illum type
            int illum = 0;

            texture map_bump;
            texture map_disp;
            texture map_decal;
            texture map_refl;

            //Roughness
            float Pr = 0.0f;
            texture map_Pr;

            //Metalic
            float Pm = 0.0f;
            texture map_Pm;

            //Sheen
            float Ps = 0.0f;
            texture map_Ps;

            //Clearcoat thickness
            float Pc = 0.0f;

            //Clearcoat roughness
            float Pcr = 0.0f;

            //Emissive
            float Ke = 0.0f;
            texture map_Ke;

            //anisotropy
            float aniso = 0.0f;
            //anisotropy rotation
            float anisor = 0.0f;

            static void parseMtl(const std::string& path, std::unordered_map<std::string, mtl>& materials);

        };
    }
}
