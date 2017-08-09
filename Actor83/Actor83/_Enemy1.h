// ---- E_Monster ----

typedef struct E_Monster_st
{
	int Color;
	int Kind;
	int AnimeAlways;

	// <-- init @ creator

	int Dir; // 2468
	int TrueFMapX;
	int TrueFMapY;
	double TrueX;
	double TrueY;
}
E_Monster_t;

E_Monster_t *E_Monster_Create(int color, int kind, int animeAlways);
void E_Monster_Release(E_Monster_t *ed);
int E_Monster_EachFrame(Enemy_t *e, E_Monster_t *ed);

// ----
