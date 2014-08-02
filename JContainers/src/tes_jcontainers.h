namespace collections {

    class tes_jcontainers : public reflection::class_meta_mixin_t<tes_jcontainers> {
    public:

        REGISTER_TES_NAME("JContainers");

        void additionalSetup() {
            metaInfo.comment = "Various utility methods";
        }

        static bool isInstalled() {
            return true;
        }
        REGISTERF2(isInstalled, nullptr, "returns true if JContainers plugin is installed");

        static UInt32 APIVersion() {
            return kJAPIVersion;
        }
        REGISTERF2(APIVersion, nullptr, []() {
            std::stringstream comm;
            comm << "returns API version. Incremented by 1 each time old API is not backward compatible with new one.\n";
            comm << "current API version is " << APIVersion();
            return comm.str();
        });

        static bool fileExistsAtPath(const char *filename) {
            if (!filename) {
                return false;
            }

            struct _stat buf;
            int result = _stat(filename, &buf);
            return result == 0;
        }
        REGISTERF2(fileExistsAtPath, "path", "returns true if file at path exists");

        static std::string userDirectory() {
            char path[MAX_PATH];
            if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, path))) {
                return std::string();
            }

            strcat_s(path, sizeof(path), JC_USER_FILES);

            if (!boost::filesystem::exists(path) && (boost::filesystem::create_directories(path), !boost::filesystem::exists(path))) {
                return std::string();
            }

            return path;
        }

        static BSFixedString _userDirectory() {
            return userDirectory().c_str();
        }
        REGISTERF(_userDirectory, "userDirectory", "", "A path to user-specific directory - "JC_USER_FILES);

        static SInt32 lastError() {
            return tes_context::instance().lastError();
        }
        REGISTERF2(lastError, nullptr, []() {
            std::stringstream comm;
            comm << "returns last occured error (error code):";
            for (int i = 0; i < JErrorCount; ++i) {
               comm << std::endl << i << " - " << JErrorCodeToString((JErrorCode)i);
            }
            return comm.str();
        });

        static const char* lastErrorString() {
            return JErrorCodeToString(tes_context::instance().lastError());
        }
        REGISTERF2(lastErrorString, nullptr, "returns string that describes last error");
    };

    TES_META_INFO(tes_jcontainers);

    TEST(tes_jcontainers, userDirectory)
    {
        auto path = tes_jcontainers::userDirectory();
        EXPECT_TRUE(!path.empty());
        EXPECT_TRUE(boost::filesystem::is_directory(path));

        auto path2 = tes_jcontainers::userDirectory() + "/MyMod/settings.json";
        tes_object::writeToFile(tes_object::object<map>(), path2.c_str());
        EXPECT_TRUE(boost::filesystem::is_regular(path2));
    }
}
