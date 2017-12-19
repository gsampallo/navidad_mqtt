#define MusicaNavidad_h

class MusicaNavidad {
  public:
    MusicaNavidad();
    void tocarMusica(int musica);
  private:
    int tonePin = D8;
    void navidad();
    void navidad1();
    void navidad2();
};
