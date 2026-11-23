#include "mac.h"

static const char kHexDigits[] = "0123456789ABCDEF";

/* Преобразует hex-цифру в значение 0..15 или возвращает -1. */
static int hex_value(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  }
  if (c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  }
  if (c >= 'a' && c <= 'f') {
    return c - 'a' + 10;
  }
  return -1;
}

int mac_serialize(const mac_addr_t *mac, char *out, size_t out_size) {
  if (mac == NULL || out == NULL || out_size < MAC_ADDR_STR_SIZE) {
    return -1;
  }

  size_t pos = 0;
  for (size_t i = 0; i < MAC_ADDR_LEN; ++i) {
    if (i != 0) {
      out[pos++] = ':';
    }
    out[pos++] = kHexDigits[(mac->octets[i] >> 4) & 0x0F];
    out[pos++] = kHexDigits[mac->octets[i] & 0x0F];
  }
  out[pos] = '\0';

  return MAC_ADDR_STR_LEN;
}

int mac_deserialize(const char *text, mac_addr_t *mac) {
  if (text == NULL || mac == NULL) {
    return -1;
  }

  /* Длина должна быть ровно MAC_ADDR_STR_LEN: символ на позиции
   * MAC_ADDR_STR_LEN обязан быть завершающим '\0'. */
  size_t len = 0;
  while (len <= MAC_ADDR_STR_LEN && text[len] != '\0') {
    ++len;
  }
  if (len != MAC_ADDR_STR_LEN) {
    return -1;
  }

  mac_addr_t parsed;
  size_t pos = 0;
  for (size_t i = 0; i < MAC_ADDR_LEN; ++i) {
    if (i != 0) {
      if (text[pos++] != ':') {
        return -1;
      }
    }
    const int hi = hex_value(text[pos++]);
    const int lo = hex_value(text[pos++]);
    if (hi < 0 || lo < 0) {
      return -1;
    }
    parsed.octets[i] = (uint8_t)((hi << 4) | lo);
  }

  *mac = parsed;
  return 0;
}
