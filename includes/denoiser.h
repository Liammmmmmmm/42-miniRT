#ifndef DENOISER_H
# define DENOISER_H

# include "basic_structs.h"
# include <stdbool.h>

# ifdef USE_OIDN
#  include <OpenImageDenoise/oidn.h>
# endif

typedef struct s_denoiser_quality
{
	bool			hdr;				// Support HDR (recommandé: true)
	bool			srgb;				// Espace couleur sRGB (false pour linear)
	bool			clean_aux;			// Nettoyage auxiliaire (true = meilleure qualité)
	int				max_memory_mb;		// Limite mémoire en MB (0 = auto)
	char			*filter_type;		// Type de filtre: "RT", "RTLightmap"
}	t_denoiser_quality;

typedef struct s_denoiser
{
	bool				enabled;			// Denoising activé/désactivé
	bool				available;			// OIDN disponible (compilé avec)
	int					width;				// Largeur de l'image
	int					height;				// Hauteur de l'image
	float				*input_buffer;		// Buffer d'entrée (RGB float)
	float				*output_buffer;		// Buffer de sortie dénoisé
	float				*albedo_buffer;		// Buffer albedo (RGB float)
	float				*normal_buffer;		// Buffer normal (RGB float)
	float				*depth_buffer;		// Buffer depth (1 canal float)
	t_denoiser_quality	quality;			// Paramètres de qualité
	bool				use_albedo;			// Utiliser le buffer albedo
	bool				use_normal;			// Utiliser le buffer normal
	bool				use_depth;			// Utiliser le buffer depth

# ifdef USE_OIDN
	OIDNDevice			device;				// Device OIDN
	OIDNFilter			filter;				// Filtre de débruitage
# else
	void				*device;			// Dummy pour compatibilité
	void				*filter;			// Dummy pour compatibilité
# endif
}	t_denoiser;

t_denoiser	*denoiser_init(int width, int height);

void		denoiser_cleanup(t_denoiser *denoiser);

void		denoiser_set_enabled(t_denoiser *denoiser, bool enabled);

void		denoiser_set_quality(t_denoiser *denoiser, bool hdr,
				bool clean_aux, const char *filter_type);

bool		denoiser_is_available(void);

float		*denoiser_process(t_denoiser *denoiser, const float *render_buffer,
				bool force);

#endif /* DENOISER_H */