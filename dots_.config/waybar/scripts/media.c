#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256

char *exec_command(const char *command)
{
  char buffer[BUFFER_SIZE];
  char *result = NULL;
  size_t len = 0;
  FILE *fp = popen(command, "r");

  if (!fp)
  {
    perror("popen");
    return NULL;
  }

  while (fgets(buffer, sizeof(buffer), fp))
  {
    size_t buffer_len = strlen(buffer);
    char *new_result = realloc(result, len + buffer_len + 1);
    if (!new_result)
    {
      perror("realloc");
      free(result);
      pclose(fp);
      return NULL;
    }
    result = new_result;
    memcpy(result + len, buffer, buffer_len);
    len += buffer_len;
    result[len] = '\0';
  }

  pclose(fp);
  return result;
}

char *get_metadata_field(const char *field)
{
  char command[BUFFER_SIZE];
  snprintf(command, sizeof(command), "playerctl metadata --format \"{{ %s }}\"", field);
  char *result = exec_command(command);

  if (result && strlen(result) > 0 && result[strlen(result) - 1] == '\n')
    result[strlen(result) - 1] = '\0';

  return result;
}

int main()
{
  char *status = exec_command("playerctl status 2>/dev/null");
  if (!status)
    return 1;

  if (strlen(status) == 0 || strstr(status, "Stopped") != NULL)
  {
    printf("{\"text\": \"\",\"class\": \"hidden\"}");
    free(status);
    return 0;
  }

  char *artist = get_metadata_field("markup_escape(artist)");
  char *album = get_metadata_field("markup_escape(album)");
  char *title = get_metadata_field("markup_escape(title)");
  char *length = get_metadata_field("duration(mpris:length)");
  char *position = get_metadata_field("duration(position)");
  char *player = get_metadata_field("markup_escape(playerName)");

  if (!artist)
    artist = strdup("Unknown");
  if (!album)
    album = strdup("Unknown");
  if (!title)
    title = strdup("Unknown");
  if (!length)
    length = strdup("Unknown");
  if (!position)
  {
    free(artist);
    free(album);
    free(title);
    free(length);
    free(player);

    return 1;
  }

  const char *icon = strstr(status, "Paused") ? "" : "";
  free(status);

  printf("{\"text\": \"%s  %s\", \"tooltip\":\"Player: %s\\nTitle: %s\\nArtist: %s\\nAlbum: %s\\nPosition: %s ・ %s\"}",
         icon, title,
         player,
         title,
         artist,
         album,
         position,
         length);

  free(artist);
  free(album);
  free(title);
  free(length);
  free(player);

  return 0;
}
