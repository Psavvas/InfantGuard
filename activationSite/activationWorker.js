const corsHeaders = {
  "Access-Control-Allow-Origin": "*",
  "Access-Control-Allow-Methods": "POST, OPTIONS",
  "Access-Control-Allow-Headers": "Content-Type",
  "Content-Type": "application/json"
};

export default {
  async fetch(request, env) {
    // Handle CORS preflight requests
    if (request.method === "OPTIONS") {
      return new Response(null, { headers: corsHeaders });
    }

    if (request.method !== "POST") {
      return new Response("Use POST", { status: 405, headers: corsHeaders });
    }

    try {
      const data = await request.json();

      const { deviceId, phoneNumber, licensePlate, vehicleMake, vehicleModel, vehicleColor } = data;

      if (!deviceId || !phoneNumber || !licensePlate || !vehicleMake || !vehicleModel || !vehicleColor) {
        return new Response(JSON.stringify({ error: "Missing fields" }), { status: 400, headers: corsHeaders });
      }

      const stmt = env.DB.prepare(
        "INSERT INTO devices (deviceId, phoneNumber, licensePlate, vehicleMake, vehicleModel, vehicleColor) VALUES (?, ?, ?, ?, ?, ?)"
      ).bind(deviceId, phoneNumber, licensePlate, vehicleMake, vehicleModel, vehicleColor);

      await stmt.run();

      return new Response(JSON.stringify({ success: true }), { headers: corsHeaders });
    } catch (error) {
      console.error("Error:", error);
      return new Response(JSON.stringify({ error: error.message }), { status: 500, headers: corsHeaders });
    }
  }
};