# Protocol giao tiếp giữa cân và cmicontroller

## Định nghĩa hằng số

1. Trạm cân

```json
{
	"TRẠM_TRƯỚC": 1,
	"TRẠM_SAU": 2
}
```

2. Giai đoạn cân

```json
{
	"FILLET": 1,
	"SỬA_CÁ": 2
}
```

3. Loại gói tin config

```json
{
	"SỐ_LÔ": 1,
	"GIAI_ĐOẠN_CÂN": 2,
	"THÀNH_PHẨM": 3
}
```

4. Trạng thái lưu dữ liệu

```json
{
	"THÀNH_CÔNG": 1,
	"THẤT_BẠI": 2
}
```

## Danh sách topic

-   `/data`: Cân gửi dữ liệu kết quả cân lên server
-   `/data/ack/id`: Server gửi phản hồi kết quả lưu dữ liệu cân nhận từ topic `/data`, trong đó `id` là id của cân
-   `/config`: Cân gửi yêu cầu lấy config lên server
-   `/config/id`: Server gửi cấu hình ca làm xuống cân, trong đó `id` là id của cân

## Cấu trúc gói tin:

1. Từ cân gửi lên server qua topic `/data`

- Với giai đoạn **Sửa cá**:

```json
{
	"k": "Mã trạm (Xem Trạm cân trong Định nghĩa hằng số)",
	"x": "Mã giai đoạn cân",
    "s": "Mã cân",
	"b": "Mã rổ",
    "w": "Khối lượng cá",
    "t": "Thời gian gửi",
	"c": "Mã lô (Chỉ gửi ở trạm trước)",
	"e": "Mã nhân viên (Chỉ gửi ở trạm trước)",
    "p": "Mã thành phẩm (chỉ gửi trạm ở trạm sau)",
}
```
*Note: Giai đoạn sửa cá có 2 gói tin ở trạm sau, 1 gói chứa khối lượng thành phẩm, 1 gói chứa khối lượng phụ phẩm. Chỉ khi nhận  được 2 gói thì quy trình mới kết thúc. Nếu nhận rổ mới khi chưa đủ 2 gói, báo lỗi thiếu data*

- Với giai đoạn **Fillet**:
```json
{
	"k": "Mã trạm (Xem Trạm cân trong Định nghĩa hằng số)",
	"x": "Mã giai đoạn cân",
    "s": "Mã cân",
    "w": "Khối lượng cá",
    "t": "Thời gian gửi",
	"c": "Mã lô (Chỉ gửi ở trạm trước)",
	"e": "Mã nhân viên (Chỉ gửi ở trạm trước)",
    "p": "Mã thành phẩm (chỉ gửi trạm ở trạm sau)",
}
```
*Note: Giai đoạn Fillet không sử dụng mã rổ, thông tin 2 gói được nối với nhau bằng mã nhân viên*

2. Từ server phản hồi xuống cân qua topic `/data/ack/id`

```json
{
    "i": "timestamp của gói",
    "s": "Trạng thái lưu dữ liệu (Xem Trạng thái lưu dữ liệu trong Định nghĩa hằng số)"
}
```

3. Cân request config qua topic `/config`

```json
{
    "i": "Mã cân",
    "t": "Loại gói tin config (Xem Loại gói tin config trong Định nghĩa hằng số)",
	"d": "Tham số (Dùng khi cần gửi thêm thông tin, VD mã giai đoạn trong config lấy thành phẩm)"
}
```

4. Config từ server gửi xuống cân qua topic `/config/id`

```json
{
    "l": "Số lượng record trong mảng d bên dưới",
    "d": [
        {
            "i": "Mã đối tượng (Dùng để gửi lên server trong gói tin)",
            "n": "Tên đối tượng (Dùng để hiển thị lên màn hình)"
        },
        {
            "i": "Mã đối tượng (Dùng để gửi lên server trong gói tin)",
            "n": "Tên đối tượng (Dùng để hiển thị lên màn hình)"
        },
       
    ]
}
```

## Gói tin mẫu

1. Cân gửi lên topic `/data`

```json
{
	"k": 1,
	"x": 2,
	"b": "0123456789AB",
	"p": 68,
	"e": "0123456789EF",
	"s": 4454,
	"w": 4.29,
	"t": 1574346370
}
```

2. Server trả về topic `/data/ack/4454`

```json
{
	"i": 1574346370,
	"s": 1
}
```

3. Cân gửi lên topic `/config`

-   Yêu cầu nhận config **Lô**

```json
{
	"i": 4545,
	"t": 1
}
```

-   Yêu cầu nhận config **Giai đoạn**

```json
{
	"i": 4545,
	"t": 2
}
```

-   Yêu cầu nhận config **Thành phẩm**

```json
{
	"i": 4545,
	"t": 3,
	"d": 2,
}
```

4. Server gửi về topic `/config/4454`:

-   Trả lời cho **Lô** (`t = 1`) trong gói request

```json
{
	"t":1,
	"l": 3,
	"d": [
		{
			"i": 1,
			"n": "Lô 1"
		},
		{
			"i": 68,
			"n": "Lô 2"
		},
		{
			"i": 98,
			"n": "Lô 3"
		}
	]
}
```

-   Trả lời cho **Giai đoạn** (`t = 2`) trong gói request

```json
{
	"t":2,
	"l": 2,
	"d": [
        {
			"i": 1,
			"n": "Cắt tiết"
		},
		{
			"i": 2,
			"n": "Fillet"
        },
	]
}
```

-   Trả lời cho **Thành phẩm** (`t = 3`) trong gói request

```json
{	
	"t":3,
	"l": 2,
	"d": [
		{
			"i": 2,
			"n": "2.1 Cá fillet"
		},
		{
			"i": 2,
			"n": "2.2 Cá xẻ bướm còn đầu"
		},
	]
}
```
