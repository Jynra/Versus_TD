/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 14:04:35 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <stdint.h>

static size_t	g_total_allocated = 0;
static size_t	g_allocation_count = 0;

void	*memory_alloc(size_t size)
{
	void	*ptr;

	if (size == 0)
	{
		debug_warning("Attempting to allocate 0 bytes");
		return (NULL);
	}
	ptr = malloc(size);
	if (!ptr)
	{
		debug_error("Failed to allocate %zu bytes", size);
		return (NULL);
	}
	g_total_allocated += size;
	g_allocation_count++;
	debug_log("Allocated %zu bytes at %p", size, ptr);
	return (ptr);
}

void	memory_free(void *ptr)
{
	uintptr_t	ptr_addr;

	if (!ptr)
	{
		debug_warning("Attempting to free NULL pointer");
		return ;
	}
	ptr_addr = (uintptr_t)ptr;
	free(ptr);
	g_allocation_count--;
	debug_log("Freed memory at %p", (void *)ptr_addr);
}

void	*memory_realloc(void *ptr, size_t size)
{
	void		*new_ptr;
	uintptr_t	old_addr;

	if (size == 0)
	{
		memory_free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (memory_alloc(size));
	old_addr = (uintptr_t)ptr;
	new_ptr = realloc(ptr, size);
	if (!new_ptr)
	{
		debug_error("Failed to reallocate %zu bytes", size);
		return (NULL);
	}
	debug_log("Reallocated %zu bytes from %p to %p", size, (void *)old_addr, new_ptr);
	return (new_ptr);
}

void	memory_set(void *ptr, int value, size_t size)
{
	if (!validate_pointer(ptr))
		return ;
	memset(ptr, value, size);
}

void	memory_copy(void *dest, const void *src, size_t size)
{
	if (!validate_pointer(dest) || !validate_pointer(src))
		return ;
	memcpy(dest, src, size);
}

int	memory_compare(const void *a, const void *b, size_t size)
{
	if (!validate_pointer(a) || !validate_pointer(b))
		return (0);
	return (memcmp(a, b, size));
}

int	string_length(const char *str)
{
	int	len;

	if (!str)
		return (0);
	len = 0;
	while (str[len])
		len++;
	return (len);
}

char	*string_copy(char *dest, const char *src)
{
	int	i;

	if (!dest || !src)
		return (NULL);
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*string_concat(char *dest, const char *src)
{
	int	dest_len;
	int	i;

	if (!dest || !src)
		return (NULL);
	dest_len = string_length(dest);
	i = 0;
	while (src[i])
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest);
}

int	string_compare(const char *a, const char *b)
{
	int	i;

	if (!a || !b)
		return (0);
	i = 0;
	while (a[i] && b[i] && a[i] == b[i])
		i++;
	return (a[i] - b[i]);
}

char	*string_find(const char *haystack, const char *needle)
{
	int	i;
	int	j;

	if (!haystack || !needle)
		return (NULL);
	if (!needle[0])
		return ((char *)haystack);
	i = 0;
	while (haystack[i])
	{
		j = 0;
		while (haystack[i + j] && needle[j] 
			&& haystack[i + j] == needle[j])
			j++;
		if (!needle[j])
			return ((char *)&haystack[i]);
		i++;
	}
	return (NULL);
}

void	string_to_lower(char *str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
		i++;
	}
}

void	string_to_upper(char *str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] -= 32;
		i++;
	}
}

bool	string_starts_with(const char *str, const char *prefix)
{
	int	i;

	if (!str || !prefix)
		return (false);
	i = 0;
	while (prefix[i])
	{
		if (str[i] != prefix[i])
			return (false);
		i++;
	}
	return (true);
}

bool	string_ends_with(const char *str, const char *suffix)
{
	int	str_len;
	int	suffix_len;
	int	i;

	if (!str || !suffix)
		return (false);
	str_len = string_length(str);
	suffix_len = string_length(suffix);
	if (suffix_len > str_len)
		return (false);
	i = 0;
	while (i < suffix_len)
	{
		if (str[str_len - suffix_len + i] != suffix[i])
			return (false);
		i++;
	}
	return (true);
}