const fontTileSize = 37;

const tileMap = [  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xFF,0xFF,0x06,0x06,0x02,0x02,0x00,0x00,
    0x00,0x00,0x00,0x00,0xAA,0x00,0x00,0x00,
    0xFF,0xFF,0x1C,0x1C,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0xAA,0x00,0x00,0x00,
    0x55,0x00,0x00,0x00,0x55,0x00,0x00,0x00,
    0x55,0x00,0x00,0x00,0x55,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x14,0x00,0x42,0x00,
    0x81,0x00,0x81,0x00,0x7E,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x0A,0x00,0x11,0x00,
    0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x60,0x00,0x90,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xFF,0xFF,0xFF,0x81,0xFF,0xFF,0x7E,0x42,
    0x7E,0x42,0x7E,0x42,0x7E,0x42,0x7E,0x42,
    0x7E,0x42,0x7E,0x42,0x7E,0x42,0x7E,0x42,
    0x7E,0x42,0x7E,0x42,0x7E,0x42,0x7E,0x42,
    0x7E,0x42,0x7E,0x42,0x7E,0x42,0x7E,0x42,
    0x7E,0x42,0xFF,0xFF,0xFF,0x81,0xFF,0xFF];

const parsed = tileMap
  .map(t => {
    const parsed = parseInt(t) + fontTileSize;

    return `0x${parsed.toString(16)}`;
  })
  .join(',');

 console.log( parsed);