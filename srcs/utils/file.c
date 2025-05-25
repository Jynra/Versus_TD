/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 13:06:12 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <sys/stat.h>
#include <unistd.h>

bool	file_exists(const char *filename)
{
	struct stat	buffer;

	if (!filename)
		return (false);
	return (stat(filename, &buffer) == 0);
}

char	*file_read_text(const char *filename)
{
	FILE	*file;
	char	*content;
	size_t	size;
	size_t	read_size;

	if (!filename || !file_exists(filename))
		return (NULL);
	file = fopen(filename, "r");
	if (!file)
	{
		debug_error("Failed to open file: %s", filename);
		return (NULL);
	}
	size = file_get_size(filename);
	content = memory_alloc(size + 1);
	if (!content)
	{
		fclose(file);
		return (NULL);
	}
	read_size = fread(content, 1, size, file);
	content[read_size] = '\0';
	fclose(file);
	debug_info("Read %zu bytes from file: %s", read_size, filename);
	return (content);
}

bool	file_write_text(const char *filename, const char *content)
{
	FILE	*file;
	size_t	content_len;
	size_t	written;

	if (!filename || !content)
		return (false);
	file = fopen(filename, "w");
	if (!file)
	{
		debug_error("Failed to create file: %s", filename);
		return (false);
	}
	content_len = string_length(content);
	written = fwrite(content, 1, content_len, file);
	fclose(file);
	if (written != content_len)
	{
		debug_error("Failed to write complete content to file: %s", filename);
		return (false);
	}
	debug_info("Wrote %zu bytes to file: %s", written, filename);
	return (true);
}

size_t	file_get_size(const char *filename)
{
	struct stat	buffer;

	if (!filename || stat(filename, &buffer) != 0)
		return (0);
	return (buffer.st_size);
}

bool	file_copy(const char *src, const char *dest)
{
	char	*content;
	bool	result;

	if (!src || !dest)
		return (false);
	if (!file_exists(src))
	{
		debug_error("Source file does not exist: %s", src);
		return (false);
	}
	content = file_read_text(src);
	if (!content)
		return (false);
	result = file_write_text(dest, content);
	memory_free(content);
	if (result)
		debug_info("File copied: %s -> %s", src, dest);
	return (result);
}

bool	file_delete(const char *filename)
{
	if (!filename)
		return (false);
	if (!file_exists(filename))
	{
		debug_warning("File does not exist: %s", filename);
		return (false);
	}
	if (unlink(filename) == 0)
	{
		debug_info("File deleted: %s", filename);
		return (true);
	}
	debug_error("Failed to delete file: %s", filename);
	return (false);
}

void	random_seed(unsigned int seed)
{
	srand(seed);
	debug_info("Random seed set to: %u", seed);
}

int	random_int(int min, int max)
{
	if (min >= max)
		return (min);
	return (min + rand() % (max - min + 1));
}

float	random_float(float min, float max)
{
	float	random_val;

	if (min >= max)
		return (min);
	random_val = (float)rand() / (float)RAND_MAX;
	return (min + random_val * (max - min));
}

bool	random_chance(float probability)
{
	if (probability <= 0.0f)
		return (false);
	if (probability >= 1.0f)
		return (true);
	return (random_float(0.0f, 1.0f) < probability);
}

t_vector2	random_vector2(float min_x, float max_x, float min_y, float max_y)
{
	t_vector2	result;

	result.x = random_float(min_x, max_x);
	result.y = random_float(min_y, max_y);
	return (result);
}

t_color	random_color(void)
{
	t_color	result;

	result.r = random_int(0, 255);
	result.g = random_int(0, 255);
	result.b = random_int(0, 255);
	result.a = 255;
	return (result);
}

t_rect	rect_create(int x, int y, int w, int h)
{
	t_rect	result;

	result.x = x;
	result.y = y;
	result.w = w;
	result.h = h;
	return (result);
}

bool	rect_contains_point(t_rect rect, t_vector2 point)
{
	return (point.x >= rect.x && point.x < rect.x + rect.w
		&& point.y >= rect.y && point.y < rect.y + rect.h);
}

bool	rect_intersects(t_rect a, t_rect b)
{
	return (a.x < b.x + b.w && a.x + a.w > b.x
		&& a.y < b.y + b.h && a.y + a.h > b.y);
}

t_rect	rect_intersection(t_rect a, t_rect b)
{
	t_rect	result;
	int		left;
	int		top;
	int		right;
	int		bottom;

	left = MAX(a.x, b.x);
	top = MAX(a.y, b.y);
	right = MIN(a.x + a.w, b.x + b.w);
	bottom = MIN(a.y + a.h, b.y + b.h);
	if (left < right && top < bottom)
	{
		result.x = left;
		result.y = top;
		result.w = right - left;
		result.h = bottom - top;
	}
	else
	{
		result.x = 0;
		result.y = 0;
		result.w = 0;
		result.h = 0;
	}
	return (result);
}

t_vector2	rect_center(t_rect rect)
{
	t_vector2	result;

	result.x = rect.x + rect.w * 0.5f;
	result.y = rect.y + rect.h * 0.5f;
	return (result);
}