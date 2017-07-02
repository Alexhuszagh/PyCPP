//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Hashlib unittests.
 */

#include <hashlib.h>
#include <gtest/gtest.h>
#include <random>
#include <utility>
#include <vector>


// FUNCTIONS
// ---------

template <typename List, typename Hasher>
static void test_digest(const List& list, const Hasher&)
{
    for (const auto &pair: list) {
        EXPECT_EQ(Hasher(pair.first.view()).hexdigest(), pair.second);
    }
}


template <typename List, typename Hasher>
static void test_update(const List& list, const Hasher&)
{
    Hasher hash("x");
    EXPECT_EQ(hash.hexdigest(), list[0]);

    hash.update("x");
    EXPECT_EQ(hash.hexdigest(), list[1]);
}


template <typename Hasher>
static void test_fuzz(const Hasher&, size_t n)
{
    std::random_device device;
    std::default_random_engine engine(device());
    std::uniform_int_distribution<> dist(-128, 127);
    for (size_t i = 0; i < 50; i++) {
        const size_t length = rand() % 1000;
        secure_string input;
        input.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            input.push_back(static_cast<char>(dist(engine)));
        }
        EXPECT_EQ(Hasher(input.view()).hexdigest().size(), n);
    }
}

// TESTS
// -----


TEST(md2, digest)
{
    std::vector<std::pair<secure_string, secure_string>> tests = {
        {
            "LOWER",
            "F6A2EBAF32C712B834BC2955EF65B575"
        },
        {
            "lower-/",
            "51E0883600D9C96A25238478EE119005"
        },
        {
            "012345",
            "D1E2CA033B157BE9B293AB08E8D3F7E0"
        },
        {
            "aaaAA0aa",
            "A6CF9BF6277C5E167AC738A259C0521B"
        },
        {
            "This is a long message",
            "710C78446AF936E31300A865F544E069"
        },
        {
            secure_string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "A61F3E492D7E68ABF93527A684D3BDC1",
        },
        {
            secure_string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "9AB540CC64A3FEEEAB112B342BFEF141",
        },
        {
            secure_string {77, -61, -86, 109, 101},
            "E6DD2B5A82E3395EE57E630CB88EB99C",
        },
    };

    test_digest(tests, md2_hash());
}


TEST(md2, update)
{
    std::vector<secure_string> tests = {
        "A0365D9BF982AAAD3526A01DB8A7206D",
        "FC770B692CAA57F6DC13FC2D1CC391A9"
    };
    test_update(tests, md2_hash());
}


TEST(md2, fuzz)
{
    test_fuzz(md2_hash(), 32);
}


TEST(md4, digest)
{
    std::vector<std::pair<secure_string, secure_string>> tests = {
        {
            "LOWER",
            "0A5F25A7ADAE4B5FC3CD6F2EC94B4D18"
        },
        {
            "lower-/",
            "430AA12ED12FE599647AF395B9B3D105"
        },
        {
            "012345",
            "9C79EAE82A4F7E7792FD5CF07A1CA21B"
        },
        {
            "aaaAA0aa",
            "0A385B1967C792C2C011ADBE4E96DD38"
        },
        {
            "This is a long message",
            "F04E9006AB9676ED48935BAEE9E67DDF"
        },
        {
            secure_string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "40D58CB3B6E382886247C08F56D1DD11",
        },
        {
            secure_string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "9FBBA5A7D9392EC44F6A60990B28F787",
        },
        {
            secure_string {77, -61, -86, 109, 101},
            "29BF61E97761AB9C9DCC7B700BCF1E7B",
        },
    };

    test_digest(tests, md4_hash());
}


TEST(md4, update)
{
    std::vector<secure_string> tests = {
        "51B834B7C1EF0B59EA50888FCB39ACE2",
        "33E0B062039C10F3F92BADEFED4A5C0B"
    };
    test_update(tests, md4_hash());
}


TEST(md4, fuzz)
{
    test_fuzz(md4_hash(), 32);
}


TEST(md5, digest)
{
    std::vector<std::pair<secure_string, secure_string>> tests = {
        {
            "LOWER",
            "A7C48BA367E019D004BFB0239B85F2B3"
        },
        {
            "lower-/",
            "72C45A43715F5D9ADFC0FCAA2F59387D"
        },
        {
            "012345",
            "D6A9A933C8AAFC51E55AC0662B6E4D4A"
        },
        {
            "aaaAA0aa",
            "6B141DFD1512CF9FC18A8A04C9323F52"
        },
        {
            "This is a long message",
            "876BDBA2E3B24196AF5AE34219316593"
        },
        {
            secure_string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "D6D014C612BE422818CA8A960063D051",
        },
        {
            secure_string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "E462805DCF84413D5EDDCA45A4B88A5E",
        },
        {
            secure_string {77, -61, -86, 109, 101},
            "89AAF570E626AC6BF001699DF83AF681",
        },
    };

    test_digest(tests, md5_hash());
}


TEST(md5, update)
{
    std::vector<secure_string> tests = {
        "9DD4E461268C8034F5C8564E155C67A6",
        "9336EBF25087D91C818EE6E9EC29F8C1"
    };
    test_update(tests, md5_hash());
}


TEST(md5, fuzz)
{
    test_fuzz(md5_hash(), 32);
}


TEST(sha1, digest)
{
    std::vector<std::pair<secure_string, secure_string>> tests = {
        {
            "LOWER",
            "E2B5C793D8E006E471A5275E68ADFB98FE059DC4"
        },
        {
            "lower-/",
            "E0EF66C0EFC8DEC254491C7FC6D0D63D272094A4"
        },
        {
            "012345",
            "FDF8BC5814536F66012884E146A8887A44709A56"
        },
        {
            "aaaAA0aa",
            "B5E88447D8D3C41D1A02CC717E2595F08A83BB29"
        },
        {
            "This is a long message",
            "57846FFA4ABAD11215011F5B5D1452F949123762"
        },
        {
            secure_string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "6E081B5948EFD1A97CECEE33A5186C2D9195A93A",
        },
        {
            secure_string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "95E2FEA39A4E4BB3A73B7CFBF82C817E91F7F5DB",
        },
        {
            secure_string {77, -61, -86, 109, 101},
            "35F7E32CF0651E8E38CCDFCECE594D4DFA34EE2B",
        },
    };

    test_digest(tests, sha1_hash());
}


TEST(sha1, update)
{
    std::vector<secure_string> tests = {
        "11F6AD8EC52A2984ABAAFD7C3B516503785C2072",
        "DD7B7B74EA160E049DD128478E074CE47254BDE8"
    };
    test_update(tests, sha1_hash());
}


TEST(sha1, fuzz)
{
    test_fuzz(sha1_hash(), 40);
}


TEST(sha2_256, digest)
{
    std::vector<std::pair<secure_string, secure_string>> tests = {
        {
            "LOWER",
            "86543EDCC2F0F4CCD0057538969B1BFF79D03F4E3CE9807194398849359D5DB0"
        },
        {
            "lower-/",
            "C8C6F2A1034E2C30F2B5758411C6A84B700FC2A3B280996342A0EFB030840146"
        },
        {
            "012345",
            "2224512EF44A62E580BB1C0DCB33AFF688F4E7DA8A488AEB4E7CA402C5CACF45"
        },
        {
            "aaaAA0aa",
            "67B542863569F340BB3CF24F7ACAE07C0721DF3740C0E8394CE8FDAB146AC9BB"
        },
        {
            "This is a long message",
            "56514B402D7BF504C2EE559075D7CE290B4AC8EDD01CDBE4238F839048E4855D",
        },
        {
            secure_string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "EA3252281BC3BCEC5672D33EC8C25DEE8ABD77235F85B8A049948F59DDF60B8D",
        },
        {
            secure_string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "9992A572307E11690B104DB56E7689EFCDDD251A363B1588209FB907F27AFA31",
        },
        {
            secure_string {77, -61, -86, 109, 101},
            "88A12C03ED7F6DE09626B834B45356DF89B1E716DBC05149C101EE14762589A4",
        }
    };

    test_digest(tests, sha2_256_hash());
}


TEST(sha2_256, update)
{
    std::vector<secure_string> tests = {
        "2D711642B726B04401627CA9FBAC32F5C8530FB1903CC4DB02258717921A4881",
        "5DDE896887F6754C9B15BFE3A441AE4806DF2FDE94001311E08BF110622E0BBE"
    };
    test_update(tests, sha2_256_hash());
}


TEST(sha2_256, fuzz)
{
    test_fuzz(sha2_256_hash(), 64);
}


TEST(sha2_224, digest)
{
    std::vector<std::pair<secure_string, secure_string>> tests = {
        {
            "LOWER",
            "C9D1EEFFAF00E89B940C9D4145B21C06705EBF4EE69E37A0990E7429"
        },
        {
            "lower-/",
            "76CABBECEE8F1C0DF0FC357C04553625A26CD367077C1A0CD11673FC"
        },
        {
            "012345",
            "7D463F13FA3D3A1525050AAEB08C8B855763ED813553175C6D1F2833"
        },
        {
            "aaaAA0aa",
            "0D5E71252EC7FD2007801F2E14BDD05B5EFFCDC131E7D2DBBD40A0B3"
        },
        {
            "This is a long message",
            "97B12CFBDCD18E948F1B359D925E9DDF74D0270AE5B8CBD06315ECB6",
        },
        {
            secure_string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "0E1F19F9DC6FB97890BB6EC7B6B25C5738B04F88508D0955DB869CD3",
        },
        {
            secure_string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "F7C152FF136E8D06DA4F48B08A36927AFAD4DB4D4470A17D74B973F0",
        },
        {
            secure_string {77, -61, -86, 109, 101},
            "93749EC518A11A7355383FCEF7146E32847E5D14D1D6B6375924318B",
        }
    };

    test_digest(tests, sha2_224_hash());
}


TEST(sha2_224, update)
{
    std::vector<secure_string> tests = {
        "54A2F7F92A5F975D8096AF77A126EDDA7DA60C5AA872EF1B871701AE",
        "F076570BF05DD6440C2B881BFF82F55F9538DE824A66BFA4A1F672B6"
    };
    test_update(tests, sha2_224_hash());
}


TEST(sha2_224, fuzz)
{
    test_fuzz(sha2_224_hash(), 56);
}


TEST(sha2_384, digest)
{
    std::vector<std::pair<secure_string, secure_string>> tests = {
        {
            "LOWER",
            "ADF848E92DFE3057BBDEE2897AB0FBE86D7AA318E5DE6CE03E6D28FEED0ECE898230CFE20E695801A1070B3EDC240826"
        },
        {
            "lower-/",
            "CC0FECD2A69A473908D0AD9A3FBF3702324CF438BBB65A06D653A040B2E6E4296C4BD78AC3BA688A8F37C08399062A4D"
        },
        {
            "012345",
            "B26834706282663258FA1A91E5BDD87A21979095E37A4D981F68FF6EADE63BBF37214C362DCF5CE1DA7FD2B08355E5AE"
        },
        {
            "aaaAA0aa",
            "B9F92C0B4B64E1D0D114802C7997B7E6C956DC17E2655F306D87BD8C4BDEF327DC8A6D5C54679C82F03F50EF1EB961BF"
        },
        {
            "This is a long message",
            "79168ABC08268FD191B36F2FA234A74834FFB284EE8C5E2BCE8661897DC2EB633818E7EC118BEE436301304DFD578829"
        },
        {
            secure_string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "81B4F40605FA52E0C6C77FA1A164E1D088B4593BD2849B252B1BD46AFEC65C3B3117D163D2F5470B60FDB5E8BBBF9B8C",
        },
        {
            secure_string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "EB85ED370F7CF979C4C3540F6C1E7BA3B64D1AA0BD27DD1B620DEF75A6B8DD75D8C8CCF9B7BA3CDCD8DB399DEC4CD690",
        },
        {
            secure_string {77, -61, -86, 109, 101},
            "93A9CA2D3F5C713E0F1E688EC3373572E80B061B30C758126864DD53237B7BA6BEBDD950ED266AF67752084156318A8C",
        },
    };

    test_digest(tests, sha2_384_hash());
}


TEST(sha2_384, update)
{
    std::vector<secure_string> tests = {
        "D752C2C51FBA0E29AA190570A9D4253E44077A058D3297FA3A5630D5BD012622F97C28ACAED313B5C83BB990CAA7DA85",
        "53062481F3D242CA7422E4DF715F24812878D1A152AE60D33BCF12A8BD9990C8359EA490BB3F420479837689C57CBD57"
    };
    test_update(tests, sha2_384_hash());
}


TEST(sha2_384, fuzz)
{
    test_fuzz(sha2_384_hash(), 96);
}


TEST(sha2_512, digest)
{
    std::vector<std::pair<secure_string, secure_string>> tests = {
        {
            "LOWER",
            "4B7A8D37DA56C3AE6DC5B8EB18D3DF3576EF972F9B876A65DEF2DB2953A14023C6BFD1159EFE8BCFFC7FA143C768A1C480E04E74B56F3F23048EED60EACE0EFC"
        },
        {
            "lower-/",
            "03C468211A527E5283A69FDDBF03EE658EB04A153B77FF07E24C478C48EAACD087A835C35A1A5EEAE8D6C4F1ABB0BE8EF3E8C9A15CD0009BC1E4166C16D178DD"
        },
        {
            "012345",
            "5DA568FFB6E614AF588F2E5DE809D3988341E8C533D8D74133843F15B246A676EECEDF78F8C44C3860AEDB04E2E6AE5C937A6960FBEA71BD47BD2ABDB01CF180"
        },
        {
            "aaaAA0aa",
            "3173C5202C5ECB8BA582999F5F8C76080A360B3DFCB06906D78140DE84BABA3E07C8DF608B6560F4998A7F77B1B2A9C3B6C706DCC0B067CF89C89CB2AAE3B21D"
        },
        {
            "This is a long message",
            "00D9225CE591FB065E2B1DEDF4FCF120601B2BDC9D05FBAE46D41D97997F37B39E2234863280D1094F756946CF9E0AD48B55DB9F9A50756D3B5CEE0669D292DC"
        },
        {
            secure_string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "3E5F2CD1EFF1775C5C759857FD5979287867027E4E469C8AF735C58165FC867F33FBF509EE58B36663A192F83CA0BA40A1F326844661D2780B7D836F2BA4E3F4",
        },
        {
            secure_string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "A033BEAE6F4D7880ECF5CF81DAA4233C3BEDB3286146BE6D923931170C299D21F6CE4EB1C5B0DC846934571292E85992DABCB0D1E2AC96F6AC54BFCF2A08011C",
        },
        {
            secure_string {77, -61, -86, 109, 101},
            "CA9DFBE61824EA2A997F7003EE18AF7F77676DB9D9BC5A2C1BA5DF1AA3C344FED590CCB7AC7D88F23DEB5B8E223BE19AEF3B87BE45642163D28D7637172ACB95",
        },
    };

    test_digest(tests, sha2_512_hash());
}


TEST(sha2_512, update)
{
    std::vector<secure_string> tests = {
        "A4ABD4448C49562D828115D13A1FCCEA927F52B4D5459297F8B43E42DA89238BC13626E43DCB38DDB082488927EC904FB42057443983E88585179D50551AFE62",
        "294C8E2D592D8B13DE92FD6D8254B33A4F4D816E06EC1C158C164A808A3D8164316908DD2580BE11660EFD8333D1F0F16B4869CB2FB94A657CFD8E3DDDBC9714"
    };
    test_update(tests, sha2_512_hash());
}


TEST(sha2_512, fuzz)
{
    test_fuzz(sha2_512_hash(), 128);
}


TEST(sha3_224, digest)
{
    std::vector<std::pair<secure_string, secure_string>> tests = {
        {
            "LOWER",
            "CB4CBAC7A0964E0983DEEAB29F00D705688708C37EFA94DA211978DE"
        },
        {
            "lower-/",
            "4AC52126A80EBECB3E5880684E24BF5ED053A896EA6FB5A27024E5F8"
        },
        {
            "012345",
            "1B2B5B0A11834BB892F116699B628E2F87EB982C909C84FD590D3D68"
        },
        {
            "aaaAA0aa",
            "E0FE22F8AB2837824CE13185DAD8B50E6739B9FBE615BC3BB4E73DF3"
        },
        {
            "This is a long message",
            "EEA114BC685943F6E57F8117D03B3136A224CDFD73946631E6485EEC"
        },
        {
            secure_string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "17BC557AEAB57A09025D9287B696506EFBFB9772F206CBA30671FF62",
        },
        {
            secure_string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "BA9ADA07D723E622900EDCE591B9B355C59C6DC243C6350ECA98E795",
        },
        {
            secure_string {77, -61, -86, 109, 101},
            "5C9AF7494944C22A7BC1E3761A1CFE878B4668CBA734F4DF5BE93CA8",
        },
    };

    test_digest(tests, sha3_224_hash());
}


TEST(sha3_224, update)
{
    std::vector<secure_string> tests = {
        "63E6CEB28AD474FA51C3D5DDA2239ADB5E58A1AE2600D18C6E116746",
        "B591DC145CC24DD0B1429E7FE2F4A3286C7F0DD957A3179145CC250B"
    };
    test_update(tests, sha3_224_hash());
}


TEST(sha3_224, fuzz)
{
    test_fuzz(sha3_224_hash(), 56);
}


TEST(sha3_256, digest)
{
    std::vector<std::pair<secure_string, secure_string>> tests = {
        {
            "LOWER",
            "7165F47727ACD26881A9B57D09AFA37DF34A62537C3F377F00FEFA66F762A2BD"
        },
        {
            "lower-/",
            "F6CF284C38D51E189D3A0C482176E76808F1771ADF1A271B70AF4D6C22B1FF24"
        },
        {
            "012345",
            "1DBAE78E7A527D6C4DE081C163C876FD52916AB42C02963B6EE2CD6212B0193E"
        },
        {
            "aaaAA0aa",
            "12ADB7E5DBD2FA65294DDCCFE0C928173081373508FBA99EE76374EB7BD634F1"
        },
        {
            "This is a long message",
            "EBC9ADB38ED64B2211E98ACEE16E3C3569B60A6DEE61281E3C4F7B407814DF66"
        },
        {
            secure_string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "DB4CDD386A63F0C890A579F3CFF40C0C69609A3738CC5FF54C9E1EE9DD3727EB",
        },
        {
            secure_string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "505910650FC5C6C09262CF7DD3DA4301F872795E43DB94213C6280FB91696755",
        },
        {
            secure_string {77, -61, -86, 109, 101},
            "BA866943702E8F6245768325E1D1226DE93DAA56DE4AEED172EA92B3C760B389",
        },
    };

    test_digest(tests, sha3_256_hash());
}


TEST(sha3_256, update)
{
    std::vector<secure_string> tests = {
        "741EFA311F97686956946758E0D95F70F11FF2DA4F2FEB7C54314F44134AC49F",
        "39E297A02806CEB43F3C6098A17352F5D4A05D60E8D19DD8E184C85FA0B6AF6D"
    };
    test_update(tests, sha3_256_hash());
}


TEST(sha3_256, fuzz)
{
    test_fuzz(sha3_256_hash(), 64);
}


TEST(sha3_384, digest)
{
    std::vector<std::pair<secure_string, secure_string>> tests = {
        {
            "LOWER",
            "E4255D285558D7E79D14B5ACBA190F511DB7B0AEFD7DCD461656E4D9507F9CBC0CF86F13BCC20118AA33D80506B2FBE3"
        },
        {
            "lower-/",
            "BD84C254A2EF1AC70CCBB062471AB99286D5BECD3AC31AEF7D1C9308370E7655164E2CA65B396CBA988F99F90C344B03"
        },
        {
            "012345",
            "4044388B06C8E35CFAA106A06A09E062CAAF7DDF51DEA592F66F56D112BFD04CA59E76AC5E848FB209998F6DB5C5BC28"
        },
        {
            "aaaAA0aa",
            "EB375B2DED011B25ADB266A0504C6EED5853A52CAEDF210032315C5D7271A8A7C7BE704A87E85CFB251494E6E90AC51B"
        },
        {
            "This is a long message",
            "6E2E13A0C063BA9166E5DF26B6C36BE0A5FCDF6DC8CF34401C50CF68BD01C46012A181EA0110B2997969903AF00D1E4F"
        },
        {
            secure_string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "FC9927AF9D1D70E81283FE6A0838922909A1634113E42A1235312DDA496F23BC57E09D518AC08ACC86CBC464214D82B9",
        },
        {
            secure_string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "EDDABAFC776165A12F8978E0269BD229EB327E7A295D2AD4EACD7DBEDCC10AA3B21F916828E6F245650FF9E0A99C9233",
        },
        {
            secure_string {77, -61, -86, 109, 101},
            "5D93A7FEFE088C5C8BFF23CDFF756AA1C1B66DB57FDB4998CE44109C711FD6668062BC9B90C60C91F87180A519179A51",
        },
    };

    test_digest(tests, sha3_384_hash());
}


TEST(sha3_384, update)
{
    std::vector<secure_string> tests = {
        "5ABFC7BC2A09A612F87987CE070634A0932D31891A61A0EC598E81E6EC616C9F00F05FF627070CBF6CB0499B1C334D4D",
        "C35C3E4D856B5117A1C0F3E9F5DEE376666A1975D447A6A57032F606BA678EECA696014324010743CEB7F625063E0138"
    };
    test_update(tests, sha3_384_hash());
}


TEST(sha3_384, fuzz)
{
    test_fuzz(sha3_384_hash(), 96);
}


TEST(sha3_512, digest)
{
    std::vector<std::pair<secure_string, secure_string>> tests = {
        {
            "LOWER",
            "AD287FDBB233F213A2D5DF760A9D232E146D732BAF19087AE8C6D75B716B91D88D811F778176BEDD0CEB3B387F8BDA6ABF805D20D6B68AD293160A859915D7AA"
        },
        {
            "lower-/",
            "A5DFBC3EE67A9624124A5C66B752A47BD5AF42816A876050D10446269A1AB915245312DE9200BAE15C6945CF00EEEF614621624FB712322E836FCFFD0E723931"
        },
        {
            "012345",
            "88D498C0D9F4D4F89DEAD26456373C7085567BAB5A81B35EA6957B6263EAD9849F65F4880AEA0612FCF3F7628B2E7724A33F9439475C5B0BA59BD2C0CA4877DF"
        },
        {
            "aaaAA0aa",
            "55EEE009BC15325A12FD906ABBBA0B22CD5168B91521EAA5E9BA8AD88EE33F7A26DD9A2707B474C117DDBF55B4F4308F86C0D1C46EC835BCC03175CFAB8E3E90"
        },
        {
            "This is a long message",
            "710A8C9E2E6D7BDC967AB6513014F47B469C932BCF54F394DEB36247FA58E191C63ACDF44E1349ACB4E771A99E68B72C3C16986045D8F9E6FEAE426DD710C58C"
        },
        {
            secure_string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "13F29C5C5EB2875A4C6B3B0D21FC00CE4CCA2E158405F8CF1F013D773CD6C4A7B61D654A8B4A1EC83B84E96C72BA3FFD076B3E82BDA527336933EC7F87CAE10B",
        },
        {
            secure_string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "6E68C5B10DA229D6D908E2356ED3BDCE0BA63C13A338D9121E9C238BBA227D9519533408773E1E6662CFD93AC3F1BD756CE2C61B3A61C6714C1051292AD79DDD",
        },
        {
            secure_string {77, -61, -86, 109, 101},
            "B96DB9BFD2953CE096A87B7BA732D471C8C8DED61CA1B676C08CFBEBC9820A65742F4133A29358EC2CADD8171C734D88B78AAA53FC338DF6D83049E21D7651BB",
        },
    };

    test_digest(tests, sha3_512_hash());
}


TEST(sha3_512, update)
{
    std::vector<secure_string> tests = {
        "0FDB27960308C51467EDD49A0F5E0C434C9CCA721F4C35BFF005FEABAF6010E777A1137EE8187C5288AF57578D18D502A0BBE4C022F5587541961E10132D9834",
        "AB585B96D532A05EBBFF8653C430099964421248AC320DDE44CAB79388A78D1D2230630E682CBE9E6EE3E2252CABBC8524FCB3B8F5A8EFE42DE278E2D33DA3B0"
    };
    test_update(tests, sha3_512_hash());
}


TEST(sha3_512, fuzz)
{
    test_fuzz(sha3_512_hash(), 128);
}


TEST(whirlpool, digest)
{
    std::vector<std::pair<secure_string, secure_string>> tests = {
        {
            "LOWER",
            "C1D36C5B5C7122371E8A027507B12A3D4921AC1DEBF8F28934100A8820B89B727EDB8E8B3A3FAA23B8C448EFB15139E366698C51EEF673A488AABFD8CC84E817"
        },
        {
            "lower-/",
            "1EB4E4070C0034C1C49A36B9D31913E7EC2682481EC851D6CB2F87B7BD0DD260FA7FD565081CC5EB9F8FF85886C918DEC790DF9F016D0BEF761161F52BD5BF78"
        },
        {
            "012345",
            "491E7948DB07297544118843396FECDE72BF091E4DCCD0FAB859B4EBC2CFA85AEA1A43B8815A000B69F9326C18D239E5B9443FAF76EA37C40418A24A1CFF3180"
        },
        {
            "aaaAA0aa",
            "A5BB6A3DB42A509C6B642820FCF8B33D34647BFF4A45EB4398F9D62BD232959714F0218DE0C24B34BB5332837E94140E1DD943B11E96B6646C3D3D14480EBA5D"
        },
        {
            "This is a long message",
            "A12DF5304F2993A1CFD35DD8691089C768B5682294892B75BD02328B9CBA85F2FB265E81031D2C95308B61208BA7BC8451C0503A76549EC99B4BED848CFED141"
        },
        {
            secure_string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "C8028FE6B56A5E549D13283388079DB8D98972423E33B5DC2EA08F02E13FE956A9BF1C6B8F4BDD66B46F0D874691E8937BB103A4BC96BF53D1C4ECC90AA1EAF3",
        },
        {
            secure_string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "4D885D54A957D760DEB37BF8C0F385AA3EE208C99A131DAA1332BD81F7E2B86FCE8F59BD0A2B3D4CBB7144A3A2333FD9A9C8F78822B1C3469EA7E2E4D9850354",
        },
        {
            secure_string {77, -61, -86, 109, 101},
            "986D4B5872F50E943DD69CD2A6D0328C3D94B4E152D301889DA68A7B31D1F531AEC76AE5CA2654195230E39CD5330226975DAFF17BAE954945FD08128E67AE7F",
        },
    };

    test_digest(tests, whirlpool_hash());
}


TEST(whirlpool, update)
{
    std::vector<secure_string> tests = {
        "7D6388114687E86BA9A4D2DCAEEB8EC10EB239FBD5B0299FECF6C1355BA6D5C603AE4294BCE70A25BEAD8E7EDAE742465554ABD643B09815D3B168BE8BC58A51",
        "76FC79712863E2A5CC3D51D92CDFE2A7958113E3DBAE99E0A605C0CC2D3267EDA6F63EA447B7434AD4D34A2A2B8B099F424D48F0DA1AD7873CEC06D6F33612DF"
    };
    test_update(tests, whirlpool_hash());
}


TEST(whirlpool, fuzz)
{
    test_fuzz(whirlpool_hash(), 128);
}


TEST(hash, digest)
{
    EXPECT_EQ(hash(md2_hash_algorithm, "LOWER").hexdigest(), "F6A2EBAF32C712B834BC2955EF65B575");
    EXPECT_EQ(hash(md4_hash_algorithm, "LOWER").hexdigest(), "0A5F25A7ADAE4B5FC3CD6F2EC94B4D18");
    EXPECT_EQ(hash(md5_hash_algorithm, "LOWER").hexdigest(), "A7C48BA367E019D004BFB0239B85F2B3");
    EXPECT_EQ(hash(sha1_hash_algorithm, "LOWER").hexdigest(), "E2B5C793D8E006E471A5275E68ADFB98FE059DC4");
    EXPECT_EQ(hash(sha2_224_hash_algorithm, "LOWER").hexdigest(), "C9D1EEFFAF00E89B940C9D4145B21C06705EBF4EE69E37A0990E7429");
    EXPECT_EQ(hash(sha2_256_hash_algorithm, "LOWER").hexdigest(), "86543EDCC2F0F4CCD0057538969B1BFF79D03F4E3CE9807194398849359D5DB0");
    EXPECT_EQ(hash(sha2_384_hash_algorithm, "LOWER").hexdigest(), "ADF848E92DFE3057BBDEE2897AB0FBE86D7AA318E5DE6CE03E6D28FEED0ECE898230CFE20E695801A1070B3EDC240826");
    EXPECT_EQ(hash(sha2_512_hash_algorithm, "LOWER").hexdigest(), "4B7A8D37DA56C3AE6DC5B8EB18D3DF3576EF972F9B876A65DEF2DB2953A14023C6BFD1159EFE8BCFFC7FA143C768A1C480E04E74B56F3F23048EED60EACE0EFC");
    EXPECT_EQ(hash(sha3_224_hash_algorithm, "LOWER").hexdigest(), "CB4CBAC7A0964E0983DEEAB29F00D705688708C37EFA94DA211978DE");
    EXPECT_EQ(hash(sha3_256_hash_algorithm, "LOWER").hexdigest(), "7165F47727ACD26881A9B57D09AFA37DF34A62537C3F377F00FEFA66F762A2BD");
    EXPECT_EQ(hash(sha3_384_hash_algorithm, "LOWER").hexdigest(), "E4255D285558D7E79D14B5ACBA190F511DB7B0AEFD7DCD461656E4D9507F9CBC0CF86F13BCC20118AA33D80506B2FBE3");
    EXPECT_EQ(hash(sha3_512_hash_algorithm, "LOWER").hexdigest(), "AD287FDBB233F213A2D5DF760A9D232E146D732BAF19087AE8C6D75B716B91D88D811F778176BEDD0CEB3B387F8BDA6ABF805D20D6B68AD293160A859915D7AA");
    EXPECT_EQ(hash(whirlpool_hash_algorithm, "LOWER").hexdigest(), "C1D36C5B5C7122371E8A027507B12A3D4921AC1DEBF8F28934100A8820B89B727EDB8E8B3A3FAA23B8C448EFB15139E366698C51EEF673A488AABFD8CC84E817");
}

