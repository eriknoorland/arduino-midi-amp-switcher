#ifndef CONFIG_H
#define CONFIG_H

class Config {
  private:
    Config();
    static Config* instance;

  public:
    static Config* getInstance() {
      if(!instance) {
        instance = new Config();
      }

      return instance;
    }

    // software version number (major.minor.hotfix)
    char VERSION_NUMBER[8] = "0.8.0";
};

#endif
